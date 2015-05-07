/* GenCyber2SLC.h generated on Sun Jul 21 09:54:27 2013
 */

#include <UdmBase.h>
#include <UdmUtil.h>
#include <cint_string.h>
#include "Cyber2SLC.h"
#include "CyberComposition.h"
#include "LINKS.h"
#include "SFC.h"

typedef std::list< Udm::Object> Packets_t;

// Forward declarations.
class TL_0;
class GetProject_5;
class CreateTypes_26;
class CreateTypesInner_2f;
class CreateStructType_46;
class StructOrMatrix_6c;
class IsStruct_87;
class IsMatrix_ac;
class CreateArrayOrBasicType_d1;
class DimensionTest_d6;
class Scalar_f5;
class ColumnVector_11c;
class Other_143;
class ProcessScalar_168;
class GetBasicType_173;
class BasicTypeExists_18e;
class Otherwise_1ba;
class UseBasicType_1df;
class CreateBasicType_20f;
class ProcessColumn_242;
class ProcessArray1_24d;
class GetArray1_25a;
class Array1Exists_282;
class Otherwise_2c0;
class UseArray1_2f5;
class CreateArray1_333;
class ProcessOther_37c;
class ProcessRowVector_387;
class ProcessArray2_392;
class CreateArray2_39f;
class UseArray2_3d5;
class GetArray2_414;
class Array2Exists_43c;
class Otherwise_47b;
class MakeAssoc_4ca;
class RegisterType_4e7;
class StructMembers_523;
class CreateStructMembers_52a;
class RegisterStruct_553;
class GetTypes_564;
class GetContainer_56d;
class GetTypes_595;
class GetTypes_NonLib_59e;
class GetTypes_Lib_5ce;
class MergeClasses_619;
class MergeClasses_622;
class MakeEquivalence_627;
class GetClasses_654;
class OneClassAtATime_671;
class MergeClasses_67b;
class ElimEquiv_680;
class GetEquivalence_68a;
class TransferSubsystems_6a7;
class TransferSubsystem_6b0;
class TransferStruct_6d4;
class TransferStruct_6db;
class TransferStructMembers_704;
class TransferTriggerMembers_70d;
class TransferTriggerMember_716;
class TransferArgDeclRef_75b;
class TransferStructMembers_782;
class TransferStructMember_78b;
class TransferParameterMembers_7da;
class TransferParameterMember_7e3;
class TransferMethodCalls_83d;
class GetMethods_846;
class TransferMethodCalls_877;
class TransferMethodCallTarget_87c;
class TransferMethodCallArgs_89d;
class TransferMethodCallArgs_8a6;
class DeleteClass_8e8;
class MarkClass_8eb;
class NoEquivDst_8fe;
class NoEquivSrc_915;
class TestEquivalence_93d;
class TestDataMembers_949;
class BreakEquivalence_94e;
class TestMembers_97a;
class ParameterMemberMatch_9a8;
class StructMemberMatch_9fb;
class NoMemberMatch_a4e;
class GetMembers_a86;
class CheckMethods_ab9;
class GetMainFunctionArgs_abe;
class BreakEquivalence_aeb;
class TestArgs_b17;
class ArgsMatch_b3f;
class NoArgMatch_b88;
class GetNextLevelClasses_bd2;
class ClassFilter_bf5;
class SimpleStructFilter_bfa;
class Marked_c0c;
class ComplexStruct1_c22;
class ComplexStruct2_c49;
class SimpleStruct_c70;
class ElimRedundantClasses_c95;
class GetClasses_c9a;
class RedundancyTest_cb7;
class NegStatementCount_cc3;
class DeleteClass_cd9;
class Constructors_cf4;
class MakeAllConstructors_cfd;
class MakeChartConstructor_d00;
class CreateInitFunction_d03;
class InitSubSubsystem_d24;
class MakeConstructors_d5a;
class InitParameters_d5f;
class InitPrimPseudoParams_d6a;
class InitPrimPseudoParam_d77;
class InitPrimitiveParameters_db6;
class ZeroPrimitiveParameter_dc3;
class GetStructMembers_dec;
class Test_e25;
class MemberHasParameter_e53;
class StructMember_e95;
class MemberNoParameter_ecd;
class InitPrimitiveParameter_f02;
class InitTriggerVars_f48;
class InitStructTrigger_f4f;
class InitStructTrigger_f56;
class InitMatrixTrigger_fa3;
class InitMatrixTrigger_fb0;
class InitDirectParameters_1005;
class InitDirectParameter_1012;
class CreateInitFunction_105a;
class MakeSFChartConstructor_1088;
class CreateInitFunction_108b;
class GetSubSubsystems_10b1;
class GetSubSubsystems_10b6;
class SubsystemFilter_10d1;
class SubsystemFilter_10da;
class IsStateChart_10ec;
class IsSFStateChart_1109;
class IsInstance_1122;
class Otherwise_1138;
class GetSubsystems_1159;
class GetSubSystems_115e;
class MakeClasses_117d;
class CreateAllPrograms_1186;
class CreateFunctionsAndArgs_118b;
class MakeChartArguments_1190;
class EventOutput_1195;
class EventOutput_119a;
class UpdateArgCount_11d3;
class DataOutput_11f9;
class DataOutput_1202;
class EventInput_1240;
class EventInput_1249;
class DataInput_1287;
class DataInput_1290;
class CreateChartFunction_12da;
class MakeArguments_1303;
class Actions_130a;
class CreateTriggerPortArgs_1313;
class AssignType_133d;
class AssignType_1342;
class ZeroUnconOutputs_1370;
class ZeroOutPort_1377;
class GetOutputPorts_1398;
class OutPortConnected_13c1;
class Connected_line_13e0;
class Connected_ConnToSFStates_140b;
class Otherwise_1437;
class Trigs_1465;
class CreateTriggerPortArgs_146e;
class Inputs_149e;
class CreateInputPortArgs_14a7;
class UpdateArgCount_14d5;
class Outputs_1512;
class CreateOutputPortArgs_151b;
class MainArgCount_1549;
class CreateFunctions_1595;
class MakeSFChartArguments_15b6;
class DataInput_15bb;
class UpdateArgCount_15c4;
class DataInput_15e4;
class EventInput_1622;
class EventInput_162b;
class DataOutput_1669;
class DataOutput_1672;
class EventOutput_16b0;
class EventOutput_16b5;
class CreateSFChartFunction_1700;
class CreateClasses_1738;
class GetSubSubsystems_173f;
class CreateChartClass_1768;
class CreateClass_1787;
class SubsystemFilter_17aa;
class HasClass_17cf;
class IsStateChart_17f6;
class IsSFStateChart_1823;
class Otherwise_1850;
class CopyClass_1875;
class CopyClass_187e;
class CreateSFChartClass_18ad;
class CreateFunctionLocalVars_18eb;
class CreateInPortLVs_18f0;
class ZeroInitLocalVars_18f7;
class GetLocalVars_1914;
class GetInArgs_192e;
class CreateLocalVars_194a;
class InPortFilter_1978;
class InPortTest_1989;
class InPort2InPort_19a8;
class OutPort2InPort_19d4;
class Otherwise_1a00;
class GetChildInPorts_1a28;
class CreateOutPortLVs_1a65;
class GetOutArgs_1a6e;
class BindToArg_1a8e;
class BindToArg_1aa1;
class OutputArgTest_1ac8;
class OutPortArg_1ae3;
class Otherwise_1b0a;
class CreateLocalVars_1b35;
class GetChildOutPorts_1b5b;
class OutPortFilter_1b88;
class OutPortTest_1b95;
class OutPort2OutPort_1bb0;
class Otherwise_1bdc;
class GetFunctions_1c12;
class GetFunctions_1c19;
class CreateProgramsTopLevel_1c42;
class HasClassFilter_1c59;
class SubsystemHasClass_1c74;
class NoClass_1c9b;
class CreateProgramClass_1cc0;
class CreateSigFlowsR_1cee;
class CreateTSB_1cf1;
class PassThroughs_1cf8;
class PassThroughs_1d01;
class GetSortedBlocks_1d37;
class DoTopologicalSort_1d64;
class CreatePreDelayExec_1d85;
class GetDelayBlocks_1d8e;
class ResetDelayBlockType_1dbe;
class CreateBlockExecution_1de2;
class TestChildBlock_1e02;
class IsChart_1e36;
class IsSFChart_1e73;
class IsSubsystem_1eac;
class Otherwise_1ee1;
class SubsystemChildBlock_1f16;
class CreateMethodCall_1f1d;
class CreateFunctionCallArgs_1f61;
class MakeTrigger_1f9f;
class IsTriggered_1fac;
class HasTriggers_1fda;
class HasActionPort_2013;
class Otherwise_204c;
class CreateTriggering_2081;
class AssignCondVal_208e;
class GetTriggerPort_20cf;
class MakeTrigger_2108;
class AddTriggerVars_2119;
class AddMatrixTriggerVars_2136;
class EitherEdge_214f;
class FallingEdge_219b;
class RisingEdge_21d9;
class EdgeTest_2217;
class RisingEdge_225d;
class FallingEdge_22b3;
class EitherEdge_2309;
class StartAssignment_235f;
class AddTriggerVar_23c4;
class AddStructTriggerVars_245d;
class EitherEdge_246a;
class FallingEdge_24c6;
class RisingEdge_2514;
class EdgeTest_2562;
class RisingEdge_25b7;
class FallingEdge_261e;
class EitherEdge_2685;
class AddTriggerVar_26ec;
class StartAssignment_2756;
class SaveState_2813;
class SaveStructState_282c;
class SaveStructState_2837;
class SaveMatrixState_288c;
class SaveMatrixState_28a1;
class AddStateVar_28ff;
class MakeCondition_296a;
class SetStructCondition_297b;
class StartCondition_2982;
class PlaceMember_29b8;
class AddTrigger_29dd;
class AddLastTrigger_29e6;
class AddNextTrigger_2a22;
class TriggerTest_2a6a;
class HasBinaryExprs_2a9f;
class Otherwise_2ae8;
class TraverseBinary_2b2d;
class GetStructMembers_2b89;
class SetMatrixCondition_2bd3;
class MatrixCondition_2be0;
class CreateConditional_2c11;
class CreateAction_2ca9;
class MakeArgDeclRef_2cf5;
class InpArgValTrigger_2cfc;
class CheckSigSrc_2d28;
class IsTrigger_2d50;
class Otherwise_2d85;
class InpArgVal_2dba;
class PrimitiveChildBlock_2e02;
class CreateCGCall_2e07;
class InitEBCall_2e10;
class GetSortedOutPorts_2e2f;
class MapParams_2e4c;
class ListOutPorts_2e61;
class GetInputs_2e80;
class ProcessMerge_2e83;
class GetMergeBlocks_2e88;
class MergeBlocksOnly_2e94;
class MergeAsSwitch_2eaa;
class GetInputPorts_2edd;
class ListInPorts_2ee0;
class GetSortedInPorts_2eff;
class CallCG_2f31;
class SetupIfVar_2f52;
class GetIfOutputPorts_2f57;
class MakeIfVar_2f94;
class GetIfAndSwitchCaseBlocks_2fc3;
class Case_2fda;
class AssignIfVar_3001;
class ChartChildBlock_305c;
class GetState_3063;
class CreateFunctionCall_30b4;
class OutputData_30fe;
class CreateOutputDataArgVal_3113;
class UpdateArgCount_3178;
class InputData_31dc;
class CreateInputDataArgVal_31f1;
class UpdateArgCount_3256;
class InputEvent_32ba;
class GetOldVar_32cf;
class MatrixInputEvent_330a;
class MatrixInputEventArgVal_3317;
class StructInputEvent_3354;
class StructInputEventArgVal_335b;
class UpdateArgCount_339d;
class OutputEvent_33e2;
class CreateOutputEventArgVal_33ed;
class MakeTrigger_3462;
class CreateTriggering_346f;
class MakeTrigger_347c;
class MakeCondition_348d;
class CreateConditional_349e;
class SetMatrixCondition_34fb;
class MatrixCondition_3508;
class SetStructCondition_3539;
class GetStructMembers_3540;
class PlaceMember_357d;
class TraverseBinary_35a2;
class TriggerTest_35ef;
class HasBinaryExprs_3624;
class Otherwise_366d;
class AddTrigger_36b2;
class AddNextTrigger_36bb;
class AddLastTrigger_36f9;
class StartCondition_374e;
class AddStateVar_379f;
class SaveState_380d;
class SaveMatrixState_3826;
class SaveMatrixState_383b;
class SaveStructState_388d;
class SaveStructState_3898;
class AddTriggerVars_38f9;
class AddStructTriggerVars_3916;
class StartAssignment_3923;
class AddTriggerVar_39b5;
class EdgeTest_3a1f;
class RisingEdge_3a74;
class FallingEdge_3adb;
class EitherEdge_3b42;
class RisingEdge_3ba9;
class FallingEdge_3bf7;
class EitherEdge_3c45;
class AddMatrixTriggerVars_3cd5;
class AddTriggerVar_3cee;
class StartAssignment_3d5a;
class EdgeTest_3dd4;
class RisingEdge_3e1a;
class FallingEdge_3e70;
class EitherEdge_3ec6;
class RisingEdge_3f1c;
class FallingEdge_3f5a;
class EitherEdge_3f98;
class GetTriggerPort_403b;
class AssignCondValLocalVar_4074;
class IsTriggered_40c3;
class HasTriggers_40eb;
class Otherwise_4124;
class SFChartChildBlock_4186;
class CreateFunctionCall_418d;
class OutputData_41d7;
class UpdateArgCount_41ec;
class CreateOutputDataArgVal_4240;
class InputData_42b5;
class UpdateArgCount_42ca;
class CreateInputDataArgVal_431e;
class OutputEvent_4393;
class CreateOutputEventArgVal_439e;
class InputEvent_4413;
class StructInputEvent_4428;
class UpdateArgCount_442f;
class StructInputEventArgVal_4460;
class MatrixInputEvent_44aa;
class MatrixInputEventArgVal_44b7;
class GetOldVar_44f4;
class MakeTrigger_453b;
class IsTriggered_4548;
class HasTriggers_4570;
class Otherwise_45a9;
class CreateTriggering_45de;
class GetTriggerPort_45eb;
class AssignCondValLocalVar_4624;
class MakeTrigger_4665;
class MakeCondition_4676;
class SetStructCondition_4687;
class StartCondition_468e;
class PlaceMember_46c4;
class AddTrigger_46e9;
class AddLastTrigger_46f2;
class AddNextTrigger_472e;
class TriggerTest_4776;
class HasBinaryExprs_47ab;
class Otherwise_47f4;
class TraverseBinary_4839;
class GetStructMembers_4895;
class SetMatrixCondition_48df;
class MatrixCondition_48ec;
class CreateConditional_491d;
class AddStateVar_4988;
class SaveState_49f6;
class SaveStructState_4a0f;
class SaveStructState_4a1a;
class SaveMatrixState_4a6f;
class SaveMatrixState_4a84;
class AddTriggerVars_4ae2;
class AddStructTriggerVars_4aff;
class EitherEdge_4b0c;
class FallingEdge_4b68;
class RisingEdge_4bb6;
class EdgeTest_4c04;
class RisingEdge_4c59;
class FallingEdge_4cc0;
class EitherEdge_4d27;
class AddTriggerVar_4d8e;
class StartAssignment_4df8;
class AddMatrixTriggerVars_4ebe;
class RisingEdge_4ed7;
class AddTriggerVar_4f15;
class EdgeTest_4f81;
class RisingEdge_4fc7;
class FallingEdge_501d;
class EitherEdge_5073;
class FallingEdge_50c9;
class EitherEdge_5107;
class StartAssignment_5153;
class GetState_523a;
class GetBlocks_52ee;
class GetSubSystems_52f7;
class FinishClasses_5329;
class GetClasses_5332;
class AddContextArgs_534f;
class AddArgValExprs_5352;
class GetContextArg_537f;
class GetAllMethodCalls_53a0;
class GetMethodCalls_53a9;
class GetSubCompounds_53e2;
class GetClassFunctions_541b;
class FinalizeContext_5458;
class AddMembers_545d;
class ContextTest_5470;
class CallerStructHasMember_548b;
class Otherwise_54b7;
class AddContextMember_54dc;
class GetCalleeObject_5506;
class AssignToOutArgs_5535;
class IdenticalOutArgs_553a;
class CheckPorts_5584;
class CheckError_558d;
class ReportError_55a4;
class CheckError_55c1;
class NoError_55dc;
class NoErrorReported_5603;
class CheckPortTypes_5630;
class CheckAllPorts_5639;
class FilterInstances_563c;
class NotAnInstanceTest_5641;
class NotAnInstance_564d;
class CheckPorts_5665;
class CheckPort_566a;
class PortHasType_5685;
class Otherwise_56b0;
class GetPorts_56d5;
class SetError_56f2;
class ChartFilter_570c;
class Block_571a;
class IsSubsystem_5737;
class GetSubBlocks_574c;
class ParamInitFcn_5777;
class CreateParamInitFcn_5780;
class CreateParamInitFcn_5783;
class CreateAssignment_57ac;
class NextLocalVar_57f1;
class TestContext_583a;
class LVinContext_5877;
class SVinContext_58bc;
class Otherwise_5901;
class LastDot_5943;
class NextDot_5976;
class SortParamRefs_59b9;
class MakeTrigger_5a27;
class IsTriggered_5a34;
class HasTriggers_5a5c;
class Otherwise_5a95;
class CreateTriggering_5aca;
class AssignCondVal_5ad7;
class CreateTriggerCondition_5b04;
class CreateTriggerOr_5b47;
class GetStructMembers_5b4e;
class PlaceMember_5b8b;
class FirstBinaryExprs_5bb0;
class CondHasExprs_5bc3;
class HasExpr_5beb;
class Otherwise_5c24;
class GetLastBinaryExprs_5c59;
class GetFirstBinaryExprs_5c66;
class GetNextBinaryExprs_5c9f;
class HasExpr_5cc7;
class Otherwise_5d00;
class CreateFirstBinaryExprs_5d45;
class CreateOnlyExprs_5d72;
class TestMemberPos_5d9e;
class NotLastMember_5dd3;
class Otherwise_5e1c;
class CreateNextBinaryExprs_5e65;
class CreateLastBinaryExprs_5e92;
class StructTest_5edb;
class IsStruct_5f03;
class Otherwise_5f38;
class ArgDeclIsCondition_5f6d;

// Class declarations.
class TL_0
{
public:
	void operator()( const Packets_t& dataflows_1, const Packets_t& projects_3);

protected:
	void callGetProject_5a12( const Packets_t& dataflows_6, const Packets_t& projects_9);
	void callCheckPorts_5a15( const Packets_t& comptypes_5585, const Packets_t& projects_5587);
	void callCreateTypes_5a18( const Packets_t& dataflows_27, const Packets_t& projects_29);
	void callMakeClasses_5a1b( const Packets_t& dataflows_117e, const Packets_t& projects_1180);
	void callFinishClasses_5a1e( const Packets_t& dataflows_532a, const Packets_t& projects_532c);
	void callMergeClasses_5a21( const Packets_t& dataflows_61a, const Packets_t& projects_61c);
	void callConstructors_5a24( const Packets_t& dataflows_cf5, const Packets_t& projects_cf7);
};

class GetProject_5
{
public:
	void operator()( const Packets_t& dataflows_6, const Packets_t& projects_9, Packets_t& dataflows_8, Packets_t& projects_b);

protected:
	bool isInputUnique( const Udm::Object& dataflow_12, const Udm::Object& project_1b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& dataflows_6, const Packets_t& projects_9);
	bool patternMatcher( const Udm::Object& dataflow_10, const Udm::Object& project_19);
	void effector();
	void forwardInputs();

private:
	Packets_t* _dataflow_c;
	Packets_t* _project_d;
	Packets_t _dataflow_e;
	Packets_t _project_17;
	class Match
	{
	public:
		CyberComposition::SimulinkWrapper dataflow_24;
		SFC::Project project_25;
	};

	std::list< Match> _matches;
};

class CreateTypes_26
{
public:
	void operator()( const Packets_t& dataflows_27, const Packets_t& projects_29, Packets_t& dataflows_2b, Packets_t& projects_2c);

protected:
	void callGetTypes_60e( const Packets_t& states_565, const Packets_t& projects_567);
	void callCreateTypesInner_611( const Packets_t& typess_30, const Packets_t& projects_32);
	void callStructMembers_614( const Packets_t& typess_524, const Packets_t& newStructs_526);
	void callRegisterStruct_617( const Packets_t& sfcStructs_554);

private:
	Packets_t* _dataflow_2d;
	Packets_t* _project_2e;
};

class CreateTypesInner_2f
{
public:
	void operator()( const Packets_t& typess_30, const Packets_t& projects_32, Packets_t& typess_34, Packets_t& newStructs_35);

protected:
	void executeOne( const Packets_t& typess_30, const Packets_t& projects_32);
	bool isInputUnique( const Udm::Object& types_3a, const Udm::Object& project_41);
	void callStructOrMatrix_51a( const Packets_t& typess_6d, const Packets_t& projects_6f);
	void callCreateArrayOrBasicType_51d( const Packets_t& typess_d2, const Packets_t& projects_d4);
	void callCreateStructType_520( const Packets_t& eSMoL_Structs_47, const Packets_t& projects_4b);

private:
	Packets_t* _types_36;
	Packets_t* _newStruct_37;
	Packets_t _types_38;
	Packets_t _project_3f;
};

class CreateStructType_46
{
public:
	void operator()( const Packets_t& eSMoL_Structs_47, const Packets_t& projects_4b, Packets_t& eSMoL_Structs_49, Packets_t& sfc_Structs_4a);

protected:
	bool isInputUnique( const Udm::Object& eSMoL_Struct_53, const Udm::Object& project_5c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& eSMoL_Structs_47, const Packets_t& projects_4b);
	bool patternMatcher( const Udm::Object& eSMoL_Struct_51, const Udm::Object& project_5a);
	void effector();
	void outputAppender( const CyberComposition::Simulink::TypeStruct& eSMoL_Struct_68, const SFC::Struct& sfc_Struct_6a);

private:
	Packets_t* _eSMoL_Struct_4d;
	Packets_t* _sfc_Struct_4e;
	Packets_t _eSMoL_Struct_4f;
	Packets_t _project_58;
	class Match
	{
	public:
		CyberComposition::Simulink::TypeStruct eSMoL_Struct_65;
		SFC::Project project_66;
	};

	std::list< Match> _matches;
};

class StructOrMatrix_6c
{
public:
	void operator()( const Packets_t& typess_6d, const Packets_t& projects_6f, Packets_t& typess_71, Packets_t& projects_72, Packets_t& typess_73, Packets_t& projects_74);

protected:
	void executeOne( const Packets_t& typess_6d, const Packets_t& projects_6f);
	bool isInputUnique( const Udm::Object& types_7b, const Udm::Object& project_82);

private:
	Packets_t* _types_75;
	Packets_t* _project_76;
	Packets_t* _types_77;
	Packets_t* _project_78;
	Packets_t _types_79;
	Packets_t _project_80;
};

class IsStruct_87
{
public:
	bool operator()( const Packets_t& typeStructs_88, const Packets_t& projects_8b, Packets_t& typeStructs_8a, Packets_t& projects_8d);

protected:
	bool isInputUnique( const Udm::Object& typeStruct_94, const Udm::Object& project_9d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& typeStructs_88, const Packets_t& projects_8b);
	bool patternMatcher( const Udm::Object& typeStruct_92, const Udm::Object& project_9b);
	void outputAppender( const CyberComposition::Simulink::TypeStruct& typeStruct_a8, const SFC::Project& project_aa);

private:
	Packets_t* _typeStruct_8e;
	Packets_t* _project_8f;
	Packets_t _typeStruct_90;
	Packets_t _project_99;
	class Match
	{
	public:
		CyberComposition::Simulink::TypeStruct typeStruct_a6;
		SFC::Project project_a7;
	};

	std::list< Match> _matches;
};

class IsMatrix_ac
{
public:
	bool operator()( const Packets_t& matrixs_ad, const Packets_t& projects_b0, Packets_t& matrixs_af, Packets_t& projects_b2);

protected:
	bool isInputUnique( const Udm::Object& matrix_b9, const Udm::Object& project_c2);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_ad, const Packets_t& projects_b0);
	bool patternMatcher( const Udm::Object& matrix_b7, const Udm::Object& project_c0);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_cd, const SFC::Project& project_cf);

private:
	Packets_t* _matrix_b3;
	Packets_t* _project_b4;
	Packets_t _matrix_b5;
	Packets_t _project_be;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_cb;
		SFC::Project project_cc;
	};

	std::list< Match> _matches;
};

class CreateArrayOrBasicType_d1
{
public:
	void operator()( const Packets_t& typess_d2, const Packets_t& projects_d4);

protected:
	void callDimensionTest_508( const Packets_t& typess_d7, const Packets_t& projects_d9);
	void callProcessScalar_50b( const Packets_t& typess_169, const Packets_t& projects_16b);
	void callProcessColumn_50e( const Packets_t& typess_243, const Packets_t& projects_245);
	void callProcessOther_511( const Packets_t& typess_37d, const Packets_t& projects_37f);
	void callRegisterType_514( const Packets_t& matrixs_4e8, const Packets_t& dTs_4eb);
	void callMakeAssoc_517( const Packets_t& matrixs_4cb, const Packets_t& dTs_4cd);
};

class DimensionTest_d6
{
public:
	void operator()( const Packets_t& typess_d7, const Packets_t& projects_d9, Packets_t& typess_db, Packets_t& projects_dc, Packets_t& typess_dd, Packets_t& projects_de, Packets_t& typess_df, Packets_t& projects_e0);

protected:
	void executeOne( const Packets_t& typess_d7, const Packets_t& projects_d9);
	bool isInputUnique( const Udm::Object& types_e9, const Udm::Object& project_f0);

private:
	Packets_t* _types_e1;
	Packets_t* _project_e2;
	Packets_t* _types_e3;
	Packets_t* _project_e4;
	Packets_t* _types_e5;
	Packets_t* _project_e6;
	Packets_t _types_e7;
	Packets_t _project_ee;
};

class Scalar_f5
{
public:
	bool operator()( const Packets_t& matrixs_f6, const Packets_t& projects_f9, Packets_t& matrixs_f8, Packets_t& projects_fb);

protected:
	bool isInputUnique( const Udm::Object& matrix_102, const Udm::Object& project_10b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_f6, const Packets_t& projects_f9);
	bool patternMatcher( const Udm::Object& matrix_100, const Udm::Object& project_109);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_118, const SFC::Project& project_11a);

private:
	Packets_t* _matrix_fc;
	Packets_t* _project_fd;
	Packets_t _matrix_fe;
	Packets_t _project_107;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_114;
		SFC::Project project_115;
	};

	std::list< Match> _matches;
};

class ColumnVector_11c
{
public:
	bool operator()( const Packets_t& matrixs_11d, const Packets_t& projects_120, Packets_t& matrixs_11f, Packets_t& projects_122);

protected:
	bool isInputUnique( const Udm::Object& matrix_129, const Udm::Object& project_132);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_11d, const Packets_t& projects_120);
	bool patternMatcher( const Udm::Object& matrix_127, const Udm::Object& project_130);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_13f, const SFC::Project& project_141);

private:
	Packets_t* _matrix_123;
	Packets_t* _project_124;
	Packets_t _matrix_125;
	Packets_t _project_12e;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_13b;
		SFC::Project project_13c;
	};

	std::list< Match> _matches;
};

class Other_143
{
public:
	bool operator()( const Packets_t& matrixs_144, const Packets_t& projects_147, Packets_t& matrixs_146, Packets_t& projects_149);

protected:
	bool isInputUnique( const Udm::Object& matrix_150, const Udm::Object& project_159);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_144, const Packets_t& projects_147);
	bool patternMatcher( const Udm::Object& matrix_14e, const Udm::Object& project_157);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_164, const SFC::Project& project_166);

private:
	Packets_t* _matrix_14a;
	Packets_t* _project_14b;
	Packets_t _matrix_14c;
	Packets_t _project_155;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_162;
		SFC::Project project_163;
	};

	std::list< Match> _matches;
};

class ProcessScalar_168
{
public:
	void operator()( const Packets_t& typess_169, const Packets_t& projects_16b, Packets_t& typess_16d, Packets_t& basictypes_16e, Packets_t& projects_16f);

protected:
	void callGetBasicType_239( const Packets_t& typess_174, const Packets_t& projects_176);
	void callUseBasicType_23c( const Packets_t& matrixs_1e0, const Packets_t& projects_1e4);
	void callCreateBasicType_23f( const Packets_t& matrixs_210, const Packets_t& projects_214);

private:
	Packets_t* _types_170;
	Packets_t* _basictype_171;
	Packets_t* _project_172;
};

class GetBasicType_173
{
public:
	void operator()( const Packets_t& typess_174, const Packets_t& projects_176, Packets_t& typess_178, Packets_t& projectss_179, Packets_t& typess_17a, Packets_t& projectss_17b);

protected:
	void executeOne( const Packets_t& typess_174, const Packets_t& projects_176);
	bool isInputUnique( const Udm::Object& types_182, const Udm::Object& project_189);

private:
	Packets_t* _types_17c;
	Packets_t* _projects_17d;
	Packets_t* _types_17e;
	Packets_t* _projects_17f;
	Packets_t _types_180;
	Packets_t _project_187;
};

class BasicTypeExists_18e
{
public:
	bool operator()( const Packets_t& matrixs_18f, const Packets_t& projects_192, Packets_t& matrixs_191, Packets_t& projects_194);

protected:
	bool isInputUnique( const Udm::Object& matrix_19b, const Udm::Object& project_1a4);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::BasicType& BasicType, CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_18f, const Packets_t& projects_192);
	bool patternMatcher( const Udm::Object& matrix_199, const Udm::Object& project_1a2);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_1b6, const SFC::Project& project_1b8);

private:
	Packets_t* _matrix_195;
	Packets_t* _project_196;
	Packets_t _matrix_197;
	Packets_t _project_1a0;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_1b0;
		SFC::Project project_1b1;
		SFC::BasicType basicType_1b2;
	};

	std::list< Match> _matches;
};

class Otherwise_1ba
{
public:
	bool operator()( const Packets_t& matrixs_1bb, const Packets_t& projects_1be, Packets_t& matrixs_1bd, Packets_t& projects_1c0);

protected:
	bool isInputUnique( const Udm::Object& matrix_1c7, const Udm::Object& project_1d0);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_1bb, const Packets_t& projects_1be);
	bool patternMatcher( const Udm::Object& matrix_1c5, const Udm::Object& project_1ce);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_1db, const SFC::Project& project_1dd);

private:
	Packets_t* _matrix_1c1;
	Packets_t* _project_1c2;
	Packets_t _matrix_1c3;
	Packets_t _project_1cc;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_1d9;
		SFC::Project project_1da;
	};

	std::list< Match> _matches;
};

class UseBasicType_1df
{
public:
	void operator()( const Packets_t& matrixs_1e0, const Packets_t& projects_1e4, Packets_t& matrixs_1e2, Packets_t& basicTypes_1e3, Packets_t& projects_1e6);

protected:
	bool isInputUnique( const Udm::Object& matrix_1ee, const Udm::Object& project_1f7);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::BasicType& BasicType, CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_1e0, const Packets_t& projects_1e4);
	bool patternMatcher( const Udm::Object& matrix_1ec, const Udm::Object& project_1f5);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_209, const SFC::BasicType& basicType_20b, const SFC::Project& project_20d);

private:
	Packets_t* _matrix_1e7;
	Packets_t* _basicType_1e8;
	Packets_t* _project_1e9;
	Packets_t _matrix_1ea;
	Packets_t _project_1f3;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_203;
		SFC::Project project_204;
		SFC::BasicType basicType_205;
	};

	std::list< Match> _matches;
};

class CreateBasicType_20f
{
public:
	void operator()( const Packets_t& matrixs_210, const Packets_t& projects_214, Packets_t& matrixs_212, Packets_t& basicTypes_213, Packets_t& projects_216);

protected:
	bool isInputUnique( const Udm::Object& matrix_21e, const Udm::Object& project_227);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_210, const Packets_t& projects_214);
	bool patternMatcher( const Udm::Object& matrix_21c, const Udm::Object& project_225);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_233, const SFC::BasicType& basicType_235, const SFC::Project& project_237);

private:
	Packets_t* _matrix_217;
	Packets_t* _basicType_218;
	Packets_t* _project_219;
	Packets_t _matrix_21a;
	Packets_t _project_223;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_230;
		SFC::Project project_231;
	};

	std::list< Match> _matches;
};

class ProcessColumn_242
{
public:
	void operator()( const Packets_t& typess_243, const Packets_t& projects_245, Packets_t& typess_247, Packets_t& array1s_248, Packets_t& projects_249);

protected:
	void callProcessScalar_375( const Packets_t& typess_169, const Packets_t& projects_16b);
	void callProcessArray1_378( const Packets_t& typess_24e, const Packets_t& basictypes_250, const Packets_t& projects_252);

private:
	Packets_t* _types_24a;
	Packets_t* _array1_24b;
	Packets_t* _project_24c;
};

class ProcessArray1_24d
{
public:
	void operator()( const Packets_t& typess_24e, const Packets_t& basictypes_250, const Packets_t& projects_252, Packets_t& typess_254, Packets_t& array1s_255, Packets_t& projects_256);

protected:
	void callGetArray1_369( const Packets_t& typess_25b, const Packets_t& dts_25d, const Packets_t& projects_25f);
	void callUseArray1_36d( const Packets_t& matrixs_2f6, const Packets_t& dTs_2f9, const Packets_t& projects_2fc);
	void callCreateArray1_371( const Packets_t& matrixs_334, const Packets_t& dTs_337, const Packets_t& projects_33a);

private:
	Packets_t* _types_257;
	Packets_t* _array1_258;
	Packets_t* _project_259;
};

class GetArray1_25a
{
public:
	void operator()( const Packets_t& typess_25b, const Packets_t& dts_25d, const Packets_t& projects_25f, Packets_t& typess_261, Packets_t& dts_262, Packets_t& projectss_263, Packets_t& typess_264, Packets_t& dts_265, Packets_t& projectss_266);

protected:
	void executeOne( const Packets_t& typess_25b, const Packets_t& dts_25d, const Packets_t& projects_25f);
	bool isInputUnique( const Udm::Object& types_26f, const Udm::Object& dt_276, const Udm::Object& project_27d);

private:
	Packets_t* _types_267;
	Packets_t* _dt_268;
	Packets_t* _projects_269;
	Packets_t* _types_26a;
	Packets_t* _dt_26b;
	Packets_t* _projects_26c;
	Packets_t _types_26d;
	Packets_t _dt_274;
	Packets_t _project_27b;
};

class Array1Exists_282
{
public:
	bool operator()( const Packets_t& matrixs_283, const Packets_t& dTs_286, const Packets_t& projects_289, Packets_t& matrixs_285, Packets_t& dTs_288, Packets_t& projects_28b);

protected:
	bool isInputUnique( const Udm::Object& matrix_293, const Udm::Object& dT_29c, const Udm::Object& project_2a5);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Array& Array, SFC::DT& DT, CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_283, const Packets_t& dTs_286, const Packets_t& projects_289);
	bool patternMatcher( const Udm::Object& matrix_291, const Udm::Object& dT_29a, const Udm::Object& project_2a3);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_2ba, const SFC::DT& dT_2bc, const SFC::Project& project_2be);

private:
	Packets_t* _matrix_28c;
	Packets_t* _dT_28d;
	Packets_t* _project_28e;
	Packets_t _matrix_28f;
	Packets_t _dT_298;
	Packets_t _project_2a1;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_2b2;
		SFC::DT dT_2b3;
		SFC::Project project_2b4;
		SFC::Array array_2b5;
	};

	std::list< Match> _matches;
};

class Otherwise_2c0
{
public:
	bool operator()( const Packets_t& matrixs_2c1, const Packets_t& dTs_2c4, const Packets_t& projects_2c7, Packets_t& matrixs_2c3, Packets_t& dTs_2c6, Packets_t& projects_2c9);

protected:
	bool isInputUnique( const Udm::Object& matrix_2d1, const Udm::Object& dT_2da, const Udm::Object& project_2e3);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_2c1, const Packets_t& dTs_2c4, const Packets_t& projects_2c7);
	bool patternMatcher( const Udm::Object& matrix_2cf, const Udm::Object& dT_2d8, const Udm::Object& project_2e1);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_2ef, const SFC::DT& dT_2f1, const SFC::Project& project_2f3);

private:
	Packets_t* _matrix_2ca;
	Packets_t* _dT_2cb;
	Packets_t* _project_2cc;
	Packets_t _matrix_2cd;
	Packets_t _dT_2d6;
	Packets_t _project_2df;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_2ec;
		SFC::DT dT_2ed;
		SFC::Project project_2ee;
	};

	std::list< Match> _matches;
};

class UseArray1_2f5
{
public:
	void operator()( const Packets_t& matrixs_2f6, const Packets_t& dTs_2f9, const Packets_t& projects_2fc, Packets_t& matrixs_2f8, Packets_t& arrays_2fb, Packets_t& projects_2fe);

protected:
	bool isInputUnique( const Udm::Object& matrix_306, const Udm::Object& dT_30f, const Udm::Object& project_318);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Array& Array, SFC::DT& DT, CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_2f6, const Packets_t& dTs_2f9, const Packets_t& projects_2fc);
	bool patternMatcher( const Udm::Object& matrix_304, const Udm::Object& dT_30d, const Udm::Object& project_316);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_32d, const SFC::Array& array_32f, const SFC::Project& project_331);

private:
	Packets_t* _matrix_2ff;
	Packets_t* _array_300;
	Packets_t* _project_301;
	Packets_t _matrix_302;
	Packets_t _dT_30b;
	Packets_t _project_314;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_325;
		SFC::DT dT_326;
		SFC::Project project_327;
		SFC::Array array_328;
	};

	std::list< Match> _matches;
};

class CreateArray1_333
{
public:
	void operator()( const Packets_t& matrixs_334, const Packets_t& dTs_337, const Packets_t& projects_33a, Packets_t& matrixs_336, Packets_t& arrays_339, Packets_t& projects_33c);

protected:
	bool isInputUnique( const Udm::Object& matrix_344, const Udm::Object& dT_34d, const Udm::Object& project_356);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_334, const Packets_t& dTs_337, const Packets_t& projects_33a);
	bool patternMatcher( const Udm::Object& matrix_342, const Udm::Object& dT_34b, const Udm::Object& project_354);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_363, const SFC::Array& array_365, const SFC::Project& project_367);

private:
	Packets_t* _matrix_33d;
	Packets_t* _array_33e;
	Packets_t* _project_33f;
	Packets_t _matrix_340;
	Packets_t _dT_349;
	Packets_t _project_352;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_35f;
		SFC::DT dT_360;
		SFC::Project project_361;
	};

	std::list< Match> _matches;
};

class ProcessOther_37c
{
public:
	void operator()( const Packets_t& typess_37d, const Packets_t& projects_37f, Packets_t& typess_381, Packets_t& array1s_382, Packets_t& projects_383);

protected:
	void callProcessRowVector_4c3( const Packets_t& typess_388, const Packets_t& projects_38a);
	void callProcessArray1_4c6( const Packets_t& typess_24e, const Packets_t& basictypes_250, const Packets_t& projects_252);

private:
	Packets_t* _types_384;
	Packets_t* _array1_385;
	Packets_t* _project_386;
};

class ProcessRowVector_387
{
public:
	void operator()( const Packets_t& typess_388, const Packets_t& projects_38a, Packets_t& typess_38c, Packets_t& array2s_38d, Packets_t& projectss_38e);

protected:
	void callProcessScalar_4bc( const Packets_t& typess_169, const Packets_t& projects_16b);
	void callProcessArray2_4bf( const Packets_t& typess_393, const Packets_t& basictypes_395, const Packets_t& projects_397);

private:
	Packets_t* _types_38f;
	Packets_t* _array2_390;
	Packets_t* _projects_391;
};

class ProcessArray2_392
{
public:
	void operator()( const Packets_t& typess_393, const Packets_t& basictypes_395, const Packets_t& projects_397, Packets_t& typess_399, Packets_t& array2s_39a, Packets_t& projectss_39b);

protected:
	void callGetArray2_4b0( const Packets_t& typess_415, const Packets_t& basictypes_417, const Packets_t& projects_419);
	void callCreateArray2_4b4( const Packets_t& matrixs_3a0, const Packets_t& basicTypes_3a4, const Packets_t& projects_3a6);
	void callUseArray2_4b8( const Packets_t& matrixs_3d6, const Packets_t& basicTypes_3da, const Packets_t& projects_3dc);

private:
	Packets_t* _types_39c;
	Packets_t* _array2_39d;
	Packets_t* _projects_39e;
};

class CreateArray2_39f
{
public:
	void operator()( const Packets_t& matrixs_3a0, const Packets_t& basicTypes_3a4, const Packets_t& projects_3a6, Packets_t& matrixs_3a2, Packets_t& arrays_3a3, Packets_t& projects_3a8);

protected:
	bool isInputUnique( const Udm::Object& matrix_3b0, const Udm::Object& basicType_3b9, const Udm::Object& project_3c2);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_3a0, const Packets_t& basicTypes_3a4, const Packets_t& projects_3a6);
	bool patternMatcher( const Udm::Object& matrix_3ae, const Udm::Object& basicType_3b7, const Udm::Object& project_3c0);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_3cf, const SFC::Array& array_3d1, const SFC::Project& project_3d3);

private:
	Packets_t* _matrix_3a9;
	Packets_t* _array_3aa;
	Packets_t* _project_3ab;
	Packets_t _matrix_3ac;
	Packets_t _basicType_3b5;
	Packets_t _project_3be;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_3cb;
		SFC::BasicType basicType_3cc;
		SFC::Project project_3cd;
	};

	std::list< Match> _matches;
};

class UseArray2_3d5
{
public:
	void operator()( const Packets_t& matrixs_3d6, const Packets_t& basicTypes_3da, const Packets_t& projects_3dc, Packets_t& matrixs_3d8, Packets_t& arrays_3d9, Packets_t& projects_3de);

protected:
	bool isInputUnique( const Udm::Object& matrix_3e6, const Udm::Object& basicType_3ef, const Udm::Object& project_3f8);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Array& Array, SFC::BasicType& BasicType, CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_3d6, const Packets_t& basicTypes_3da, const Packets_t& projects_3dc);
	bool patternMatcher( const Udm::Object& matrix_3e4, const Udm::Object& basicType_3ed, const Udm::Object& project_3f6);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_40e, const SFC::Array& array_410, const SFC::Project& project_412);

private:
	Packets_t* _matrix_3df;
	Packets_t* _array_3e0;
	Packets_t* _project_3e1;
	Packets_t _matrix_3e2;
	Packets_t _basicType_3eb;
	Packets_t _project_3f4;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_406;
		SFC::BasicType basicType_407;
		SFC::Project project_408;
		SFC::Array array_409;
	};

	std::list< Match> _matches;
};

class GetArray2_414
{
public:
	void operator()( const Packets_t& typess_415, const Packets_t& basictypes_417, const Packets_t& projects_419, Packets_t& typess_41b, Packets_t& basictypes_41c, Packets_t& projectss_41d, Packets_t& typess_41e, Packets_t& basictypes_41f, Packets_t& projectss_420);

protected:
	void executeOne( const Packets_t& typess_415, const Packets_t& basictypes_417, const Packets_t& projects_419);
	bool isInputUnique( const Udm::Object& types_429, const Udm::Object& basictype_430, const Udm::Object& project_437);

private:
	Packets_t* _types_421;
	Packets_t* _basictype_422;
	Packets_t* _projects_423;
	Packets_t* _types_424;
	Packets_t* _basictype_425;
	Packets_t* _projects_426;
	Packets_t _types_427;
	Packets_t _basictype_42e;
	Packets_t _project_435;
};

class Array2Exists_43c
{
public:
	bool operator()( const Packets_t& matrixs_43d, const Packets_t& basicTypes_440, const Packets_t& projects_443, Packets_t& matrixs_43f, Packets_t& basicTypes_442, Packets_t& projects_445);

protected:
	bool isInputUnique( const Udm::Object& matrix_44d, const Udm::Object& basicType_456, const Udm::Object& project_45f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Array& Array, SFC::BasicType& BasicType, CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_43d, const Packets_t& basicTypes_440, const Packets_t& projects_443);
	bool patternMatcher( const Udm::Object& matrix_44b, const Udm::Object& basicType_454, const Udm::Object& project_45d);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_475, const SFC::BasicType& basicType_477, const SFC::Project& project_479);

private:
	Packets_t* _matrix_446;
	Packets_t* _basicType_447;
	Packets_t* _project_448;
	Packets_t _matrix_449;
	Packets_t _basicType_452;
	Packets_t _project_45b;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_46d;
		SFC::BasicType basicType_46e;
		SFC::Project project_46f;
		SFC::Array array_470;
	};

	std::list< Match> _matches;
};

class Otherwise_47b
{
public:
	bool operator()( const Packets_t& matrixs_47c, const Packets_t& basicTypes_47f, const Packets_t& projects_482, Packets_t& matrixs_47e, Packets_t& basicTypes_481, Packets_t& projects_484);

protected:
	bool isInputUnique( const Udm::Object& matrix_48c, const Udm::Object& basicType_495, const Udm::Object& project_49e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_47c, const Packets_t& basicTypes_47f, const Packets_t& projects_482);
	bool patternMatcher( const Udm::Object& matrix_48a, const Udm::Object& basicType_493, const Udm::Object& project_49c);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_4aa, const SFC::BasicType& basicType_4ac, const SFC::Project& project_4ae);

private:
	Packets_t* _matrix_485;
	Packets_t* _basicType_486;
	Packets_t* _project_487;
	Packets_t _matrix_488;
	Packets_t _basicType_491;
	Packets_t _project_49a;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_4a7;
		SFC::BasicType basicType_4a8;
		SFC::Project project_4a9;
	};

	std::list< Match> _matches;
};

class MakeAssoc_4ca
{
public:
	void operator()( const Packets_t& matrixs_4cb, const Packets_t& dTs_4cd);

protected:
	bool isInputUnique( const Udm::Object& matrix_4d3, const Udm::Object& dT_4dc);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_4cb, const Packets_t& dTs_4cd);
	bool patternMatcher( const Udm::Object& matrix_4d1, const Udm::Object& dT_4da);
	void effector();

private:
	Packets_t _matrix_4cf;
	Packets_t _dT_4d8;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_4e5;
		SFC::DT dT_4e6;
	};

	std::list< Match> _matches;
};

class RegisterType_4e7
{
public:
	void operator()( const Packets_t& matrixs_4e8, const Packets_t& dTs_4eb, Packets_t& matrixs_4ea, Packets_t& dTs_4ed);

protected:
	bool isInputUnique( const Udm::Object& matrix_4f4, const Udm::Object& dT_4fd);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_4e8, const Packets_t& dTs_4eb);
	bool patternMatcher( const Udm::Object& matrix_4f2, const Udm::Object& dT_4fb);
	void effector();
	void forwardInputs();

private:
	Packets_t* _matrix_4ee;
	Packets_t* _dT_4ef;
	Packets_t _matrix_4f0;
	Packets_t _dT_4f9;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_506;
		SFC::DT dT_507;
	};

	std::list< Match> _matches;
};

class StructMembers_523
{
public:
	void operator()( const Packets_t& typess_524, const Packets_t& newStructs_526, Packets_t& newStructs_528);

protected:
	void callCreateStructMembers_550( const Packets_t& eSMoL_Structs_52b, const Packets_t& sfc_Structs_52d);

private:
	Packets_t* _newStruct_529;
};

class CreateStructMembers_52a
{
public:
	void operator()( const Packets_t& eSMoL_Structs_52b, const Packets_t& sfc_Structs_52d);

protected:
	bool isInputUnique( const Udm::Object& eSMoL_Struct_533, const Udm::Object& sfc_Struct_53c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& eSMoL_Structs_52b, const Packets_t& sfc_Structs_52d);
	bool patternMatcher( const Udm::Object& eSMoL_Struct_531, const Udm::Object& sfc_Struct_53a);
	void effector();

private:
	Packets_t _eSMoL_Struct_52f;
	Packets_t _sfc_Struct_538;
	class Match
	{
	public:
		CyberComposition::Simulink::TypeStruct eSMoL_Struct_54a;
		SFC::Struct sfc_Struct_54b;
		CyberComposition::Simulink::TypeBaseRef eSMoL_StructRef_54c;
		CyberComposition::Simulink::SF_TypeBase eSMoL_TypeBase_54d;
		SFC::DT sfc_DT_54e;
	};

	std::list< Match> _matches;
};

class RegisterStruct_553
{
public:
	void operator()( const Packets_t& sfcStructs_554);

protected:
	bool isInputUnique( const Udm::Object& sfcStruct_55a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sfcStructs_554);
	bool patternMatcher( const Udm::Object& sfcStruct_558);
	void effector();

private:
	Packets_t _sfcStruct_556;
	class Match
	{
	public:
		SFC::Struct sfcStruct_563;
	};

	std::list< Match> _matches;
};

class GetTypes_564
{
public:
	void operator()( const Packets_t& states_565, const Packets_t& projects_567, Packets_t& typess_569, Packets_t& projects_56a);

protected:
	void callGetContainer_608( const Packets_t& dataflows_56f, const Packets_t& projects_571);
	void callGetTypes_60b( const Packets_t& containers_596, const Packets_t& projects_598);

private:
	Packets_t* _types_56b;
	Packets_t* _project_56c;
};

class GetContainer_56d
{
public:
	void operator()( const Packets_t& dataflows_56f, const Packets_t& projects_571, Packets_t& designFolders_56e, Packets_t& projects_573);

protected:
	bool isInputUnique( const Udm::Object& dataflow_57a, const Udm::Object& project_583);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& dataflows_56f, const Packets_t& projects_571);
	bool patternMatcher( const Udm::Object& dataflow_578, const Udm::Object& project_581);
	void effector();
	void outputAppender( const CyberComposition::Components& designFolder_591, const SFC::Project& project_593);

private:
	Packets_t* _designFolder_574;
	Packets_t* _project_575;
	Packets_t _dataflow_576;
	Packets_t _project_57f;
	class Match
	{
	public:
		CyberComposition::SimulinkWrapper dataflow_58e;
		SFC::Project project_58f;
		CyberComposition::Components designFolder_590;
	};

	std::list< Match> _matches;
};

class GetTypes_595
{
public:
	void operator()( const Packets_t& containers_596, const Packets_t& projects_598, Packets_t& typess_59a, Packets_t& projects_59b);

protected:
	void callGetTypes_NonLib_602( const Packets_t& designFolders_59f, const Packets_t& projects_5a2);
	void callGetTypes_Lib_605( const Packets_t& designFolders_5cf, const Packets_t& projects_5d2);

private:
	Packets_t* _types_59c;
	Packets_t* _project_59d;
};

class GetTypes_NonLib_59e
{
public:
	void operator()( const Packets_t& designFolders_59f, const Packets_t& projects_5a2, Packets_t& typeBases_5a1, Packets_t& projects_5a4);

protected:
	bool isInputUnique( const Udm::Object& designFolder_5ab, const Udm::Object& project_5b4);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& designFolders_59f, const Packets_t& projects_5a2);
	bool patternMatcher( const Udm::Object& designFolder_5a9, const Udm::Object& project_5b2);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_TypeBase& typeBase_5ca, const SFC::Project& project_5cc);

private:
	Packets_t* _typeBase_5a5;
	Packets_t* _project_5a6;
	Packets_t _designFolder_5a7;
	Packets_t _project_5b0;
	class Match
	{
	public:
		CyberComposition::Components designFolder_5c5;
		SFC::Project project_5c6;
		CyberComposition::RootFolder rootFolder_5c7;
		CyberComposition::Simulink::Types types_5c8;
		CyberComposition::Simulink::SF_TypeBase typeBase_5c9;
	};

	std::list< Match> _matches;
};

class GetTypes_Lib_5ce
{
public:
	void operator()( const Packets_t& designFolders_5cf, const Packets_t& projects_5d2, Packets_t& typeBases_5d1, Packets_t& projects_5d4);

protected:
	bool isInputUnique( const Udm::Object& designFolder_5db, const Udm::Object& project_5e4);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& designFolders_5cf, const Packets_t& projects_5d2);
	bool patternMatcher( const Udm::Object& designFolder_5d9, const Udm::Object& project_5e2);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_TypeBase& typeBase_5fe, const SFC::Project& project_600);

private:
	Packets_t* _typeBase_5d5;
	Packets_t* _project_5d6;
	Packets_t _designFolder_5d7;
	Packets_t _project_5e0;
	class Match
	{
	public:
		CyberComposition::Components designFolder_5f8;
		SFC::Project project_5f9;
		CyberComposition::RootFolder rootFolder_5fa;
		CyberComposition::RootFolder rootFolder_5fb;
		CyberComposition::Simulink::Types types_5fc;
		CyberComposition::Simulink::SF_TypeBase typeBase_5fd;
	};

	std::list< Match> _matches;
};

class MergeClasses_619
{
public:
	void operator()( const Packets_t& dataflows_61a, const Packets_t& projects_61c, Packets_t& dataflows_61e, Packets_t& projects_61f);

protected:
	void callMergeClasses_cf0( const Packets_t& projects_623);
	void callElimRedundantClasses_cf2( const Packets_t& projects_c96);

private:
	Packets_t* _dataflow_620;
	Packets_t* _project_621;
};

class MergeClasses_622
{
public:
	void operator()( const Packets_t& projects_623, Packets_t& projects_625);

protected:
	void callMakeEquivalence_c8f( const Packets_t& projects_628);
	void callGetClasses_c91( const Packets_t& projects_656);
	void callOneClassAtATime_c93( const Packets_t& classs_672);

private:
	Packets_t* _project_626;
};

class MakeEquivalence_627
{
public:
	void operator()( const Packets_t& projects_628, Packets_t& projects_62a);

protected:
	bool isInputUnique( const Udm::Object& project_630);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Class& Class1, SFC::Class& Class2, SFC::Program& Program, SFC::Project& Project, CyberComposition::Simulink::Subsystem& Subsystem1, CyberComposition::Simulink::Subsystem& Subsystem2);
	void processInputPackets( const Packets_t& projects_628);
	bool patternMatcher( const Udm::Object& project_62e);
	void effector();
	void forwardInputs();

private:
	Packets_t* _project_62b;
	Packets_t _project_62c;
	class Match
	{
	public:
		SFC::Project project_648;
		SFC::Program program_649;
		SFC::Class class1_64a;
		SFC::Class class2_64b;
		CyberComposition::Simulink::Subsystem subsystem1_64c;
		CyberComposition::Simulink::Subsystem subsystem2_64d;
	};

	std::list< Match> _matches;
};

class GetClasses_654
{
public:
	void operator()( const Packets_t& projects_656, Packets_t& classs_655);

protected:
	bool isInputUnique( const Udm::Object& project_65d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& projects_656);
	bool patternMatcher( const Udm::Object& project_65b);
	void effector();
	void outputAppender( const SFC::Class& class_66f);

private:
	Packets_t* _class_658;
	Packets_t _project_659;
	class Match
	{
	public:
		SFC::Project project_66c;
		SFC::Program program_66d;
		SFC::Class class_66e;
	};

	std::list< Match> _matches;
};

class OneClassAtATime_671
{
public:
	void operator()( const Packets_t& classs_672);

protected:
	void executeOne( const Packets_t& classs_672);
	bool isInputUnique( const Udm::Object& class_676);
	void callClassFilter_c87( const Packets_t& classs_bf6);
	void callMergeClasses_c89( const Packets_t& classs_67c);
	void callGetNextLevelClasses_c8b( const Packets_t& childClasss_bd3);
	void callOneClassAtATime_c8d( const Packets_t& classs_672);

private:
	Packets_t _class_674;
};

class MergeClasses_67b
{
public:
	void operator()( const Packets_t& classs_67c, Packets_t& classs_67e);

protected:
	void callTestEquivalence_bce( const Packets_t& classs_93e);
	void callElimEquiv_bd0( const Packets_t& classs_681);

private:
	Packets_t* _class_67f;
};

class ElimEquiv_680
{
public:
	void operator()( const Packets_t& classs_681);

protected:
	void executeOne( const Packets_t& classs_681);
	bool isInputUnique( const Udm::Object& class_685);
	void callGetEquivalence_930( const Packets_t& class1s_68b);
	void callTransferSubsystems_932( const Packets_t& class1s_6a8, const Packets_t& class2s_6aa);
	void callTransferMethodCalls_935( const Packets_t& class1s_83e, const Packets_t& class2s_840);
	void callTransferStruct_938( const Packets_t& class1s_6d5, const Packets_t& class2s_6d7);
	void callDeleteClass_93b( const Packets_t& deleteClasss_8e9);

private:
	Packets_t _class_683;
};

class GetEquivalence_68a
{
public:
	void operator()( const Packets_t& class1s_68b, Packets_t& class1s_68d, Packets_t& class2s_68e);

protected:
	bool isInputUnique( const Udm::Object& class1_695);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& class1s_68b);
	bool patternMatcher( const Udm::Object& class1_693);
	void effector();
	void outputAppender( const SFC::Class& class1_6a3, const SFC::Class& class2_6a5);

private:
	Packets_t* _class1_68f;
	Packets_t* _class2_690;
	Packets_t _class1_691;
	class Match
	{
	public:
		SFC::Class class1_6a1;
		SFC::Class class2_6a2;
	};

	std::list< Match> _matches;
};

class TransferSubsystems_6a7
{
public:
	void operator()( const Packets_t& class1s_6a8, const Packets_t& class2s_6aa, Packets_t& class1s_6ac, Packets_t& class2s_6ad);

protected:
	void callTransferSubsystem_6d1( const Packets_t& class1s_6b1, const Packets_t& class2s_6b3);

private:
	Packets_t* _class1_6ae;
	Packets_t* _class2_6af;
};

class TransferSubsystem_6b0
{
public:
	void operator()( const Packets_t& class1s_6b1, const Packets_t& class2s_6b3);

protected:
	bool isInputUnique( const Udm::Object& class1_6b9, const Udm::Object& class2_6c2);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& class1s_6b1, const Packets_t& class2s_6b3);
	bool patternMatcher( const Udm::Object& class1_6b7, const Udm::Object& class2_6c0);
	void effector();

private:
	Packets_t _class1_6b5;
	Packets_t _class2_6be;
	class Match
	{
	public:
		SFC::Class class1_6ce;
		SFC::Class class2_6cf;
		CyberComposition::Simulink::Subsystem subsystem_6d0;
	};

	std::list< Match> _matches;
};

class TransferStruct_6d4
{
public:
	void operator()( const Packets_t& class1s_6d5, const Packets_t& class2s_6d7, Packets_t& deleteclasss_6d9);

protected:
	void callTransferStructMembers_837( const Packets_t& class1s_705, const Packets_t& class2s_707);
	void callTransferStruct_83a( const Packets_t& class1s_6dc, const Packets_t& class2s_6de);

private:
	Packets_t* _deleteclass_6da;
};

class TransferStruct_6db
{
public:
	void operator()( const Packets_t& class1s_6dc, const Packets_t& class2s_6de);

protected:
	bool isInputUnique( const Udm::Object& class1_6e4, const Udm::Object& class2_6ed);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& class1s_6dc, const Packets_t& class2s_6de);
	bool patternMatcher( const Udm::Object& class1_6e2, const Udm::Object& class2_6eb);
	void effector();

private:
	Packets_t _class1_6e0;
	Packets_t _class2_6e9;
	class Match
	{
	public:
		SFC::Class class1_6ff;
		SFC::Class class2_700;
		SFC::Struct struct1_701;
		SFC::ArgDeclBase argDeclBase_702;
		SFC::Struct struct2_703;
	};

	std::list< Match> _matches;
};

class TransferStructMembers_704
{
public:
	void operator()( const Packets_t& class1s_705, const Packets_t& class2s_707, Packets_t& class1s_709, Packets_t& class2s_70a);

protected:
	void callTransferParameterMembers_82e( const Packets_t& class1s_7db, const Packets_t& class2s_7dd);
	void callTransferTriggerMembers_831( const Packets_t& class1s_70e, const Packets_t& class2s_710);
	void callTransferStructMembers_834( const Packets_t& class1s_783, const Packets_t& class2s_785);

private:
	Packets_t* _class1_70b;
	Packets_t* _class2_70c;
};

class TransferTriggerMembers_70d
{
public:
	void operator()( const Packets_t& class1s_70e, const Packets_t& class2s_710, Packets_t& class1s_712, Packets_t& class2s_713);

protected:
	void callTransferTriggerMember_77c( const Packets_t& class1s_717, const Packets_t& class2s_71a);
	void callTransferArgDeclRef_77f( const Packets_t& localVar1s_75c, const Packets_t& localVar2s_75e);

private:
	Packets_t* _class1_714;
	Packets_t* _class2_715;
};

class TransferTriggerMember_716
{
public:
	void operator()( const Packets_t& class1s_717, const Packets_t& class2s_71a, Packets_t& localVar1s_719, Packets_t& localVar2s_71c);

protected:
	bool isInputUnique( const Udm::Object& class1_723, const Udm::Object& class2_72c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Class& Class1, SFC::Class& Class2, SFC::LocalVar& LocalVar1, SFC::LocalVar& LocalVar2, SFC::Struct& Struct1, SFC::Struct& Struct2, CyberComposition::Simulink::TriggerPort& TriggerPort1, CyberComposition::Simulink::TriggerPort& TriggerPort2);
	void processInputPackets( const Packets_t& class1s_717, const Packets_t& class2s_71a);
	bool patternMatcher( const Udm::Object& class1_721, const Udm::Object& class2_72a);
	void effector();
	void outputAppender( const SFC::LocalVar& localVar1_757, const SFC::LocalVar& localVar2_759);

private:
	Packets_t* _localVar1_71d;
	Packets_t* _localVar2_71e;
	Packets_t _class1_71f;
	Packets_t _class2_728;
	class Match
	{
	public:
		SFC::Class class1_747;
		SFC::Class class2_748;
		SFC::Struct struct1_749;
		SFC::LocalVar localVar1_74a;
		CyberComposition::Simulink::TriggerPort triggerPort2_74b;
		CyberComposition::Simulink::TriggerPort triggerPort1_74c;
		SFC::Struct struct2_74d;
		SFC::LocalVar localVar2_74e;
	};

	std::list< Match> _matches;
};

class TransferArgDeclRef_75b
{
public:
	void operator()( const Packets_t& localVar1s_75c, const Packets_t& localVar2s_75e);

protected:
	bool isInputUnique( const Udm::Object& localVar1_764, const Udm::Object& localVar2_76d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& localVar1s_75c, const Packets_t& localVar2s_75e);
	bool patternMatcher( const Udm::Object& localVar1_762, const Udm::Object& localVar2_76b);
	void effector();

private:
	Packets_t _localVar1_760;
	Packets_t _localVar2_769;
	class Match
	{
	public:
		SFC::LocalVar localVar1_779;
		SFC::LocalVar localVar2_77a;
		SFC::ArgDeclRef argDeclRef_77b;
	};

	std::list< Match> _matches;
};

class TransferStructMembers_782
{
public:
	void operator()( const Packets_t& class1s_783, const Packets_t& class2s_785, Packets_t& class1s_787, Packets_t& class2s_788);

protected:
	void callTransferStructMember_7d4( const Packets_t& class1s_78c, const Packets_t& class2s_78f);
	void callTransferArgDeclRef_7d7( const Packets_t& localVar1s_75c, const Packets_t& localVar2s_75e);

private:
	Packets_t* _class1_789;
	Packets_t* _class2_78a;
};

class TransferStructMember_78b
{
public:
	void operator()( const Packets_t& class1s_78c, const Packets_t& class2s_78f, Packets_t& localVar1s_78e, Packets_t& localVar2s_791);

protected:
	bool isInputUnique( const Udm::Object& class1_798, const Udm::Object& class2_7a1);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Class& Class, SFC::Class& Class1, SFC::Class& Class2, SFC::LocalVar& LocalVar1, SFC::LocalVar& LocalVar2, SFC::Struct& Struct1, SFC::Struct& Struct2, CyberComposition::Simulink::Subsystem& SubSubsystem1, CyberComposition::Simulink::Subsystem& SubSubsystem2);
	void processInputPackets( const Packets_t& class1s_78c, const Packets_t& class2s_78f);
	bool patternMatcher( const Udm::Object& class1_796, const Udm::Object& class2_79f);
	void effector();
	void outputAppender( const SFC::LocalVar& localVar1_7d0, const SFC::LocalVar& localVar2_7d2);

private:
	Packets_t* _localVar1_792;
	Packets_t* _localVar2_793;
	Packets_t _class1_794;
	Packets_t _class2_79d;
	class Match
	{
	public:
		SFC::Class class1_7be;
		SFC::Class class2_7bf;
		SFC::Struct struct1_7c0;
		SFC::LocalVar localVar1_7c1;
		CyberComposition::Simulink::Subsystem subSubsystem1_7c2;
		SFC::Class class_7c3;
		CyberComposition::Simulink::Subsystem subSubsystem2_7c4;
		SFC::Struct struct2_7c5;
		SFC::LocalVar localVar2_7c6;
	};

	std::list< Match> _matches;
};

class TransferParameterMembers_7da
{
public:
	void operator()( const Packets_t& class1s_7db, const Packets_t& class2s_7dd, Packets_t& class1s_7df, Packets_t& class2s_7e0);

protected:
	void callTransferParameterMember_828( const Packets_t& class1s_7e4, const Packets_t& class2s_7e7);
	void callTransferArgDeclRef_82b( const Packets_t& localVar1s_75c, const Packets_t& localVar2s_75e);

private:
	Packets_t* _class1_7e1;
	Packets_t* _class2_7e2;
};

class TransferParameterMember_7e3
{
public:
	void operator()( const Packets_t& class1s_7e4, const Packets_t& class2s_7e7, Packets_t& localVar1s_7e6, Packets_t& localVar2s_7e9);

protected:
	bool isInputUnique( const Udm::Object& class1_7f0, const Udm::Object& class2_7f9);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Class& Class1, SFC::Class& Class2, SFC::LocalVar& LocalVar1, SFC::LocalVar& LocalVar2, CyberComposition::Simulink::SF_Parameter& Parameter1, CyberComposition::Simulink::SF_Parameter& Parameter2, SFC::Struct& Struct1, SFC::Struct& Struct2);
	void processInputPackets( const Packets_t& class1s_7e4, const Packets_t& class2s_7e7);
	bool patternMatcher( const Udm::Object& class1_7ee, const Udm::Object& class2_7f7);
	void effector();
	void outputAppender( const SFC::LocalVar& localVar1_824, const SFC::LocalVar& localVar2_826);

private:
	Packets_t* _localVar1_7ea;
	Packets_t* _localVar2_7eb;
	Packets_t _class1_7ec;
	Packets_t _class2_7f5;
	class Match
	{
	public:
		SFC::Class class1_814;
		SFC::Class class2_815;
		SFC::Struct struct1_816;
		SFC::LocalVar localVar1_817;
		CyberComposition::Simulink::SF_Parameter parameter1_818;
		CyberComposition::Simulink::SF_Parameter parameter2_819;
		SFC::Struct struct2_81a;
		SFC::LocalVar localVar2_81b;
	};

	std::list< Match> _matches;
};

class TransferMethodCalls_83d
{
public:
	void operator()( const Packets_t& class1s_83e, const Packets_t& class2s_840, Packets_t& class1s_842, Packets_t& class2s_843);

protected:
	void callGetMethods_8e2( const Packets_t& class1s_848, const Packets_t& class2s_84b);
	void callTransferMethodCalls_8e5( const Packets_t& method1s_878, const Packets_t& method2s_87a);

private:
	Packets_t* _class1_844;
	Packets_t* _class2_845;
};

class GetMethods_846
{
public:
	void operator()( const Packets_t& class1s_848, const Packets_t& class2s_84b, Packets_t& method1s_847, Packets_t& method2s_84a);

protected:
	bool isInputUnique( const Udm::Object& class1_853, const Udm::Object& class2_85c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Class& Class1, SFC::Class& Class2, SFC::Function& Method1, SFC::Function& Method2);
	void processInputPackets( const Packets_t& class1s_848, const Packets_t& class2s_84b);
	bool patternMatcher( const Udm::Object& class1_851, const Udm::Object& class2_85a);
	void effector();
	void outputAppender( const SFC::Function& method1_873, const SFC::Function& method2_875);

private:
	Packets_t* _method1_84d;
	Packets_t* _method2_84e;
	Packets_t _class1_84f;
	Packets_t _class2_858;
	class Match
	{
	public:
		SFC::Class class1_86b;
		SFC::Class class2_86c;
		SFC::Function method1_86d;
		SFC::Function method2_86e;
	};

	std::list< Match> _matches;
};

class TransferMethodCalls_877
{
public:
	void operator()( const Packets_t& method1s_878, const Packets_t& method2s_87a);

protected:
	void callTransferMethodCallArgs_8dc( const Packets_t& method1s_89e, const Packets_t& method2s_8a0);
	void callTransferMethodCallTarget_8df( const Packets_t& method1s_87d, const Packets_t& method2s_87f);
};

class TransferMethodCallTarget_87c
{
public:
	void operator()( const Packets_t& method1s_87d, const Packets_t& method2s_87f);

protected:
	bool isInputUnique( const Udm::Object& method1_885, const Udm::Object& method2_88e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& method1s_87d, const Packets_t& method2s_87f);
	bool patternMatcher( const Udm::Object& method1_883, const Udm::Object& method2_88c);
	void effector();

private:
	Packets_t _method1_881;
	Packets_t _method2_88a;
	class Match
	{
	public:
		SFC::Function method1_89a;
		SFC::Function method2_89b;
		SFC::FunctionCall methodCall2_89c;
	};

	std::list< Match> _matches;
};

class TransferMethodCallArgs_89d
{
public:
	void operator()( const Packets_t& method1s_89e, const Packets_t& method2s_8a0, Packets_t& method1s_8a2, Packets_t& method2s_8a3);

protected:
	void callTransferMethodCallArgs_8d9( const Packets_t& method1s_8a7, const Packets_t& method2s_8a9);

private:
	Packets_t* _method1_8a4;
	Packets_t* _method2_8a5;
};

class TransferMethodCallArgs_8a6
{
public:
	void operator()( const Packets_t& method1s_8a7, const Packets_t& method2s_8a9);

protected:
	bool isInputUnique( const Udm::Object& method1_8af, const Udm::Object& method2_8b8);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg1, SFC::Arg& Arg2, SFC::ArgVal& ArgVal, SFC::Function& Method1, SFC::Function& Method2, SFC::FunctionCall& MethodCall2);
	void processInputPackets( const Packets_t& method1s_8a7, const Packets_t& method2s_8a9);
	bool patternMatcher( const Udm::Object& method1_8ad, const Udm::Object& method2_8b6);
	void effector();

private:
	Packets_t _method1_8ab;
	Packets_t _method2_8b4;
	class Match
	{
	public:
		SFC::Function method1_8cd;
		SFC::Function method2_8ce;
		SFC::Arg arg1_8cf;
		SFC::FunctionCall methodCall2_8d0;
		SFC::ArgVal argVal_8d1;
		SFC::Arg arg2_8d2;
	};

	std::list< Match> _matches;
};

class DeleteClass_8e8
{
public:
	void operator()( const Packets_t& deleteClasss_8e9);

protected:
	void callmarkClass_92a( const Packets_t& classs_8ec);
	void callnoEquivDst_92c( const Packets_t& class2s_8ff);
	void callnoEquivSrc_92e( const Packets_t& class1s_916);
};

class MarkClass_8eb
{
public:
	void operator()( const Packets_t& classs_8ec, Packets_t& classs_8ee);

protected:
	bool isInputUnique( const Udm::Object& class_8f4);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& classs_8ec);
	bool patternMatcher( const Udm::Object& class_8f2);
	void effector();
	void forwardInputs();

private:
	Packets_t* _class_8ef;
	Packets_t _class_8f0;
	class Match
	{
	public:
		SFC::Class class_8fd;
	};

	std::list< Match> _matches;
};

class NoEquivDst_8fe
{
public:
	void operator()( const Packets_t& class2s_8ff, Packets_t& class2s_901);

protected:
	bool isInputUnique( const Udm::Object& class2_907);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& class2s_8ff);
	bool patternMatcher( const Udm::Object& class2_905);
	void effector();
	void forwardInputs();

private:
	Packets_t* _class2_902;
	Packets_t _class2_903;
	class Match
	{
	public:
		SFC::Class class2_913;
		SFC::Class class1_914;
	};

	std::list< Match> _matches;
};

class NoEquivSrc_915
{
public:
	void operator()( const Packets_t& class1s_916);

protected:
	bool isInputUnique( const Udm::Object& class1_91c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& class1s_916);
	bool patternMatcher( const Udm::Object& class1_91a);
	void effector();

private:
	Packets_t _class1_918;
	class Match
	{
	public:
		SFC::Class class1_928;
		SFC::Class class2_929;
	};

	std::list< Match> _matches;
};

class TestEquivalence_93d
{
public:
	void operator()( const Packets_t& classs_93e, Packets_t& classs_940);

protected:
	void executeOne( const Packets_t& classs_93e);
	bool isInputUnique( const Udm::Object& class_944);
	void callTestDataMembers_bca( const Packets_t& classs_94a);
	void callCheckMethods_bcc( const Packets_t& classs_aba);

private:
	Packets_t* _class_941;
	Packets_t _class_942;
};

class TestDataMembers_949
{
public:
	void operator()( const Packets_t& classs_94a, Packets_t& classs_94c);

protected:
	void callGetMembers_aaf( const Packets_t& class1s_a87);
	void callTestMembers_ab1( const Packets_t& class1s_97b, const Packets_t& class2s_97d, const Packets_t& members_97f);
	void callBreakEquivalence_ab5( const Packets_t& class1s_94f, const Packets_t& class2s_951, const Packets_t& localVar1s_953);

private:
	Packets_t* _class_94d;
};

class BreakEquivalence_94e
{
public:
	void operator()( const Packets_t& class1s_94f, const Packets_t& class2s_951, const Packets_t& localVar1s_953);

protected:
	bool isInputUnique( const Udm::Object& class1_959, const Udm::Object& class2_962, const Udm::Object& localVar1_96b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& class1s_94f, const Packets_t& class2s_951, const Packets_t& localVar1s_953);
	bool patternMatcher( const Udm::Object& class1_957, const Udm::Object& class2_960, const Udm::Object& localVar1_969);
	void effector();

private:
	Packets_t _class1_955;
	Packets_t _class2_95e;
	Packets_t _localVar1_967;
	class Match
	{
	public:
		SFC::Class class1_977;
		SFC::Class class2_978;
		SFC::LocalVar localVar1_979;
	};

	std::list< Match> _matches;
};

class TestMembers_97a
{
public:
	void operator()( const Packets_t& class1s_97b, const Packets_t& class2s_97d, const Packets_t& members_97f, Packets_t& class1s_981, Packets_t& class2s_982, Packets_t& members_983, Packets_t& class1s_984, Packets_t& class2s_985, Packets_t& members_986, Packets_t& class1s_987, Packets_t& class2s_988, Packets_t& members_989);

protected:
	void executeOne( const Packets_t& class1s_97b, const Packets_t& class2s_97d, const Packets_t& members_97f);
	bool isInputUnique( const Udm::Object& class1_995, const Udm::Object& class2_99c, const Udm::Object& member_9a3);

private:
	Packets_t* _class1_98a;
	Packets_t* _class2_98b;
	Packets_t* _member_98c;
	Packets_t* _class1_98d;
	Packets_t* _class2_98e;
	Packets_t* _member_98f;
	Packets_t* _class1_990;
	Packets_t* _class2_991;
	Packets_t* _member_992;
	Packets_t _class1_993;
	Packets_t _class2_99a;
	Packets_t _member_9a1;
};

class ParameterMemberMatch_9a8
{
public:
	bool operator()( const Packets_t& class1s_9a9, const Packets_t& class2s_9ac, const Packets_t& localVar1s_9af, Packets_t& class1s_9ab, Packets_t& class2s_9ae, Packets_t& localVar1s_9b1);

protected:
	bool isInputUnique( const Udm::Object& class1_9b9, const Udm::Object& class2_9c2, const Udm::Object& localVar1_9cb);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Class& Class1, SFC::Class& Class2, SFC::DT& DT, SFC::LocalVar& LocalVar1, SFC::LocalVar& LocalVar2, CyberComposition::Simulink::SF_Parameter& Parameter1, CyberComposition::Simulink::SF_Parameter& Parameter2, SFC::Struct& Struct2);
	void processInputPackets( const Packets_t& class1s_9a9, const Packets_t& class2s_9ac, const Packets_t& localVar1s_9af);
	bool patternMatcher( const Udm::Object& class1_9b7, const Udm::Object& class2_9c0, const Udm::Object& localVar1_9c9);
	void outputAppender( const SFC::Class& class1_9f5, const SFC::Class& class2_9f7, const SFC::LocalVar& localVar1_9f9);

private:
	Packets_t* _class1_9b2;
	Packets_t* _class2_9b3;
	Packets_t* _localVar1_9b4;
	Packets_t _class1_9b5;
	Packets_t _class2_9be;
	Packets_t _localVar1_9c7;
	class Match
	{
	public:
		SFC::Class class1_9e5;
		SFC::Class class2_9e6;
		SFC::LocalVar localVar1_9e7;
		CyberComposition::Simulink::SF_Parameter parameter2_9e8;
		SFC::Struct struct2_9e9;
		SFC::LocalVar localVar2_9ea;
		SFC::DT dT_9eb;
		CyberComposition::Simulink::SF_Parameter parameter1_9ec;
	};

	std::list< Match> _matches;
};

class StructMemberMatch_9fb
{
public:
	bool operator()( const Packets_t& class1s_9fc, const Packets_t& class2s_9ff, const Packets_t& localVar1s_a02, Packets_t& class1s_9fe, Packets_t& class2s_a01, Packets_t& localVar1s_a04);

protected:
	bool isInputUnique( const Udm::Object& class1_a0c, const Udm::Object& class2_a15, const Udm::Object& localVar1_a1e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Class& Class, SFC::Class& Class1, SFC::Class& Class2, SFC::LocalVar& LocalVar1, SFC::LocalVar& LocalVar2, SFC::Struct& Struct2, CyberComposition::Simulink::Subsystem& Subsystem1, CyberComposition::Simulink::Subsystem& Subsystem2);
	void processInputPackets( const Packets_t& class1s_9fc, const Packets_t& class2s_9ff, const Packets_t& localVar1s_a02);
	bool patternMatcher( const Udm::Object& class1_a0a, const Udm::Object& class2_a13, const Udm::Object& localVar1_a1c);
	void outputAppender( const SFC::Class& class1_a48, const SFC::Class& class2_a4a, const SFC::LocalVar& localVar1_a4c);

private:
	Packets_t* _class1_a05;
	Packets_t* _class2_a06;
	Packets_t* _localVar1_a07;
	Packets_t _class1_a08;
	Packets_t _class2_a11;
	Packets_t _localVar1_a1a;
	class Match
	{
	public:
		SFC::Class class1_a38;
		SFC::Class class2_a39;
		SFC::LocalVar localVar1_a3a;
		SFC::Struct struct2_a3b;
		SFC::LocalVar localVar2_a3c;
		SFC::Class class_a3d;
		CyberComposition::Simulink::Subsystem subsystem1_a3e;
		CyberComposition::Simulink::Subsystem subsystem2_a3f;
	};

	std::list< Match> _matches;
};

class NoMemberMatch_a4e
{
public:
	bool operator()( const Packets_t& class1s_a4f, const Packets_t& class2s_a52, const Packets_t& localVar1s_a55, Packets_t& class1s_a51, Packets_t& class2s_a54, Packets_t& localVar1s_a57);

protected:
	bool isInputUnique( const Udm::Object& class1_a5f, const Udm::Object& class2_a68, const Udm::Object& localVar1_a71);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& class1s_a4f, const Packets_t& class2s_a52, const Packets_t& localVar1s_a55);
	bool patternMatcher( const Udm::Object& class1_a5d, const Udm::Object& class2_a66, const Udm::Object& localVar1_a6f);
	void outputAppender( const SFC::Class& class1_a80, const SFC::Class& class2_a82, const SFC::LocalVar& localVar1_a84);

private:
	Packets_t* _class1_a58;
	Packets_t* _class2_a59;
	Packets_t* _localVar1_a5a;
	Packets_t _class1_a5b;
	Packets_t _class2_a64;
	Packets_t _localVar1_a6d;
	class Match
	{
	public:
		SFC::Class class1_a7d;
		SFC::Class class2_a7e;
		SFC::LocalVar localVar1_a7f;
	};

	std::list< Match> _matches;
};

class GetMembers_a86
{
public:
	void operator()( const Packets_t& class1s_a87, Packets_t& class1s_a89, Packets_t& class2s_a8a, Packets_t& localVar1s_a8b);

protected:
	bool isInputUnique( const Udm::Object& class1_a93);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& class1s_a87);
	bool patternMatcher( const Udm::Object& class1_a91);
	void effector();
	void outputAppender( const SFC::Class& class1_aa9, const SFC::Class& class2_aab, const SFC::LocalVar& localVar1_aad);

private:
	Packets_t* _class1_a8c;
	Packets_t* _class2_a8d;
	Packets_t* _localVar1_a8e;
	Packets_t _class1_a8f;
	class Match
	{
	public:
		SFC::Class class1_aa5;
		SFC::Class class2_aa6;
		SFC::LocalVar localVar1_aa7;
		SFC::Struct classStruct1_aa8;
	};

	std::list< Match> _matches;
};

class CheckMethods_ab9
{
public:
	void operator()( const Packets_t& classs_aba, Packets_t& classs_abc);

protected:
	void callGetMainFunctionArgs_bc0( const Packets_t& class1s_abf);
	void callTestArgs_bc2( const Packets_t& class1s_b18, const Packets_t& class2s_b1a, const Packets_t& args_b1c);
	void callBreakEquivalence_bc6( const Packets_t& class1s_aec, const Packets_t& class2s_aee, const Packets_t& args_af0);

private:
	Packets_t* _class_abd;
};

class GetMainFunctionArgs_abe
{
public:
	void operator()( const Packets_t& class1s_abf, Packets_t& class1s_ac1, Packets_t& class2s_ac2, Packets_t& args_ac3);

protected:
	bool isInputUnique( const Udm::Object& class1_acb);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg, SFC::Class& Class1, SFC::Class& Class2, SFC::Function& MainFunction1);
	void processInputPackets( const Packets_t& class1s_abf);
	bool patternMatcher( const Udm::Object& class1_ac9);
	void effector();
	void outputAppender( const SFC::Class& class1_ae5, const SFC::Class& class2_ae7, const SFC::Arg& arg_ae9);

private:
	Packets_t* _class1_ac4;
	Packets_t* _class2_ac5;
	Packets_t* _arg_ac6;
	Packets_t _class1_ac7;
	class Match
	{
	public:
		SFC::Class class1_add;
		SFC::Class class2_ade;
		SFC::Arg arg_adf;
		SFC::Function mainFunction1_ae0;
	};

	std::list< Match> _matches;
};

class BreakEquivalence_aeb
{
public:
	void operator()( const Packets_t& class1s_aec, const Packets_t& class2s_aee, const Packets_t& args_af0);

protected:
	bool isInputUnique( const Udm::Object& class1_af6, const Udm::Object& class2_aff, const Udm::Object& arg_b08);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& class1s_aec, const Packets_t& class2s_aee, const Packets_t& args_af0);
	bool patternMatcher( const Udm::Object& class1_af4, const Udm::Object& class2_afd, const Udm::Object& arg_b06);
	void effector();

private:
	Packets_t _class1_af2;
	Packets_t _class2_afb;
	Packets_t _arg_b04;
	class Match
	{
	public:
		SFC::Class class1_b14;
		SFC::Class class2_b15;
		SFC::Arg arg_b16;
	};

	std::list< Match> _matches;
};

class TestArgs_b17
{
public:
	void operator()( const Packets_t& class1s_b18, const Packets_t& class2s_b1a, const Packets_t& args_b1c, Packets_t& class1s_b1e, Packets_t& class2s_b1f, Packets_t& args_b20, Packets_t& class1s_b21, Packets_t& class2s_b22, Packets_t& args_b23);

protected:
	void executeOne( const Packets_t& class1s_b18, const Packets_t& class2s_b1a, const Packets_t& args_b1c);
	bool isInputUnique( const Udm::Object& class1_b2c, const Udm::Object& class2_b33, const Udm::Object& arg_b3a);

private:
	Packets_t* _class1_b24;
	Packets_t* _class2_b25;
	Packets_t* _arg_b26;
	Packets_t* _class1_b27;
	Packets_t* _class2_b28;
	Packets_t* _arg_b29;
	Packets_t _class1_b2a;
	Packets_t _class2_b31;
	Packets_t _arg_b38;
};

class ArgsMatch_b3f
{
public:
	bool operator()( const Packets_t& class1s_b40, const Packets_t& class2s_b43, const Packets_t& arg1s_b46, Packets_t& class1s_b42, Packets_t& class2s_b45, Packets_t& arg1s_b48);

protected:
	bool isInputUnique( const Udm::Object& class1_b50, const Udm::Object& class2_b59, const Udm::Object& arg1_b62);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg1, SFC::Arg& Arg2, SFC::Class& Class1, SFC::Class& Class2, SFC::DT& DT, SFC::Function& MainFunction2);
	void processInputPackets( const Packets_t& class1s_b40, const Packets_t& class2s_b43, const Packets_t& arg1s_b46);
	bool patternMatcher( const Udm::Object& class1_b4e, const Udm::Object& class2_b57, const Udm::Object& arg1_b60);
	void outputAppender( const SFC::Class& class1_b82, const SFC::Class& class2_b84, const SFC::Arg& arg1_b86);

private:
	Packets_t* _class1_b49;
	Packets_t* _class2_b4a;
	Packets_t* _arg1_b4b;
	Packets_t _class1_b4c;
	Packets_t _class2_b55;
	Packets_t _arg1_b5e;
	class Match
	{
	public:
		SFC::Class class1_b76;
		SFC::Class class2_b77;
		SFC::Arg arg1_b78;
		SFC::Arg arg2_b79;
		SFC::Function mainFunction2_b7a;
		SFC::DT dT_b7b;
	};

	std::list< Match> _matches;
};

class NoArgMatch_b88
{
public:
	bool operator()( const Packets_t& class1s_b89, const Packets_t& class2s_b8c, const Packets_t& arg1s_b8f, Packets_t& class1s_b8b, Packets_t& class2s_b8e, Packets_t& arg1s_b91);

protected:
	bool isInputUnique( const Udm::Object& class1_b99, const Udm::Object& class2_ba2, const Udm::Object& arg1_bab);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& class1s_b89, const Packets_t& class2s_b8c, const Packets_t& arg1s_b8f);
	bool patternMatcher( const Udm::Object& class1_b97, const Udm::Object& class2_ba0, const Udm::Object& arg1_ba9);
	void outputAppender( const SFC::Class& class1_bba, const SFC::Class& class2_bbc, const SFC::Arg& arg1_bbe);

private:
	Packets_t* _class1_b92;
	Packets_t* _class2_b93;
	Packets_t* _arg1_b94;
	Packets_t _class1_b95;
	Packets_t _class2_b9e;
	Packets_t _arg1_ba7;
	class Match
	{
	public:
		SFC::Class class1_bb7;
		SFC::Class class2_bb8;
		SFC::Arg arg1_bb9;
	};

	std::list< Match> _matches;
};

class GetNextLevelClasses_bd2
{
public:
	void operator()( const Packets_t& childClasss_bd3, Packets_t& parentClasss_bd5);

protected:
	bool isInputUnique( const Udm::Object& childClass_bdb);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& childClasss_bd3);
	bool patternMatcher( const Udm::Object& childClass_bd9);
	void effector();
	void outputAppender( const SFC::Class& parentClass_bf3);

private:
	Packets_t* _parentClass_bd6;
	Packets_t _childClass_bd7;
	class Match
	{
	public:
		SFC::Class childClass_bee;
		SFC::Class parentClass_bef;
		SFC::LocalVar member_bf0;
		SFC::Struct childStruct_bf1;
		SFC::Struct parentStruct_bf2;
	};

	std::list< Match> _matches;
};

class ClassFilter_bf5
{
public:
	void operator()( const Packets_t& classs_bf6, Packets_t& classs_bf8);

protected:
	void callSimpleStructFilter_c85( const Packets_t& classs_bfb);

private:
	Packets_t* _class_bf9;
};

class SimpleStructFilter_bfa
{
public:
	void operator()( const Packets_t& classs_bfb, Packets_t& classs_bfd, Packets_t& classs_bfe, Packets_t& classs_bff, Packets_t& classs_c00);

protected:
	void executeOne( const Packets_t& classs_bfb);
	bool isInputUnique( const Udm::Object& class_c07);

private:
	Packets_t* _class_c01;
	Packets_t* _class_c02;
	Packets_t* _class_c03;
	Packets_t* _class_c04;
	Packets_t _class_c05;
};

class Marked_c0c
{
public:
	bool operator()( const Packets_t& classs_c0d, Packets_t& classs_c0f);

protected:
	bool isInputUnique( const Udm::Object& class_c15);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Class& Class);
	void processInputPackets( const Packets_t& classs_c0d);
	bool patternMatcher( const Udm::Object& class_c13);
	void outputAppender( const SFC::Class& class_c20);

private:
	Packets_t* _class_c10;
	Packets_t _class_c11;
	class Match
	{
	public:
		SFC::Class class_c1e;
	};

	std::list< Match> _matches;
};

class ComplexStruct1_c22
{
public:
	bool operator()( const Packets_t& classs_c23, Packets_t& classs_c25);

protected:
	bool isInputUnique( const Udm::Object& class_c2b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& classs_c23);
	bool patternMatcher( const Udm::Object& class_c29);
	void outputAppender( const SFC::Class& class_c47);

private:
	Packets_t* _class_c26;
	Packets_t _class_c27;
	class Match
	{
	public:
		SFC::Class class_c41;
		SFC::Class childClass_c42;
		SFC::Class equivClass_c43;
		SFC::Struct parentStruct_c44;
		SFC::LocalVar localVar_c45;
		SFC::Struct childStruct_c46;
	};

	std::list< Match> _matches;
};

class ComplexStruct2_c49
{
public:
	bool operator()( const Packets_t& classs_c4a, Packets_t& classs_c4c);

protected:
	bool isInputUnique( const Udm::Object& class_c52);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& classs_c4a);
	bool patternMatcher( const Udm::Object& class_c50);
	void outputAppender( const SFC::Class& class_c6e);

private:
	Packets_t* _class_c4d;
	Packets_t _class_c4e;
	class Match
	{
	public:
		SFC::Class class_c68;
		SFC::Struct parentStruct_c69;
		SFC::Class childClass_c6a;
		SFC::Class equivClass_c6b;
		SFC::LocalVar localVar_c6c;
		SFC::Struct childStruct_c6d;
	};

	std::list< Match> _matches;
};

class SimpleStruct_c70
{
public:
	bool operator()( const Packets_t& classs_c71, Packets_t& classs_c73);

protected:
	bool isInputUnique( const Udm::Object& class_c79);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& classs_c71);
	bool patternMatcher( const Udm::Object& class_c77);
	void outputAppender( const SFC::Class& class_c83);

private:
	Packets_t* _class_c74;
	Packets_t _class_c75;
	class Match
	{
	public:
		SFC::Class class_c82;
	};

	std::list< Match> _matches;
};

class ElimRedundantClasses_c95
{
public:
	void operator()( const Packets_t& projects_c96, Packets_t& projects_c98);

protected:
	void callGetClasses_cea( const Packets_t& projects_c9c);
	void callRedundancyTest_cec( const Packets_t& classs_cb8);
	void callDeleteClass_cee( const Packets_t& classs_cda);

private:
	Packets_t* _project_c99;
};

class GetClasses_c9a
{
public:
	void operator()( const Packets_t& projects_c9c, Packets_t& classs_c9b);

protected:
	bool isInputUnique( const Udm::Object& project_ca3);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& projects_c9c);
	bool patternMatcher( const Udm::Object& project_ca1);
	void effector();
	void outputAppender( const SFC::Class& class_cb5);

private:
	Packets_t* _class_c9e;
	Packets_t _project_c9f;
	class Match
	{
	public:
		SFC::Project project_cb2;
		SFC::Program program_cb3;
		SFC::Class class_cb4;
	};

	std::list< Match> _matches;
};

class RedundancyTest_cb7
{
public:
	void operator()( const Packets_t& classs_cb8, Packets_t& classs_cba);

protected:
	void executeOne( const Packets_t& classs_cb8);
	bool isInputUnique( const Udm::Object& class_cbe);

private:
	Packets_t* _class_cbb;
	Packets_t _class_cbc;
};

class NegStatementCount_cc3
{
public:
	bool operator()( const Packets_t& classs_cc4, Packets_t& classs_cc6);

protected:
	bool isInputUnique( const Udm::Object& class_ccc);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Class& Class);
	void processInputPackets( const Packets_t& classs_cc4);
	bool patternMatcher( const Udm::Object& class_cca);
	void outputAppender( const SFC::Class& class_cd7);

private:
	Packets_t* _class_cc7;
	Packets_t _class_cc8;
	class Match
	{
	public:
		SFC::Class class_cd5;
	};

	std::list< Match> _matches;
};

class DeleteClass_cd9
{
public:
	void operator()( const Packets_t& classs_cda);

protected:
	bool isInputUnique( const Udm::Object& class_ce0);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& classs_cda);
	bool patternMatcher( const Udm::Object& class_cde);
	void effector();

private:
	Packets_t _class_cdc;
	class Match
	{
	public:
		SFC::Class class_ce9;
	};

	std::list< Match> _matches;
};

class Constructors_cf4
{
public:
	void operator()( const Packets_t& dataflows_cf5, const Packets_t& projects_cf7, Packets_t& dataflows_cf9, Packets_t& projects_cfa);

protected:
	void callGetSubsystems_1179( const Packets_t& comptypes_115a);
	void callMakeAllConstructors_117b( const Packets_t& subsystems_cfe);

private:
	Packets_t* _dataflow_cfb;
	Packets_t* _project_cfc;
};

class MakeAllConstructors_cfd
{
public:
	void operator()( const Packets_t& subsystems_cfe);

protected:
	void callSubsystemFilter_114f( const Packets_t& systems_10d2);
	void callMakeChartConstructor_1151( const Packets_t& systems_d01);
	void callMakeConstructors_1153( const Packets_t& systems_d5b);
	void callMakeSFChartConstructor_1155( const Packets_t& systems_1089);
	void callGetSubSubsystems_1157( const Packets_t& systems_10b2);
};

class MakeChartConstructor_d00
{
public:
	void operator()( const Packets_t& systems_d01);

protected:
	void callCreateInitFunction_d55( const Packets_t& subsystems_d04);
	void callinitSubSubsystem_d57( const Packets_t& subsystems_d25, const Packets_t& initFunctions_d27);
};

class CreateInitFunction_d03
{
public:
	void operator()( const Packets_t& subsystems_d04, Packets_t& subsystems_d06, Packets_t& initFunctions_d07);

protected:
	bool isInputUnique( const Udm::Object& subsystem_d0e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_d04);
	bool patternMatcher( const Udm::Object& subsystem_d0c);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_d20, const SFC::Function& initFunction_d22);

private:
	Packets_t* _subsystem_d08;
	Packets_t* _initFunction_d09;
	Packets_t _subsystem_d0a;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_d1b;
		SFC::Class class_d1c;
		SFC::Struct struct_d1d;
	};

	std::list< Match> _matches;
};

class InitSubSubsystem_d24
{
public:
	void operator()( const Packets_t& subsystems_d25, const Packets_t& initFunctions_d27);

protected:
	bool isInputUnique( const Udm::Object& subsystem_d2d, const Udm::Object& initFunction_d36);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_d25, const Packets_t& initFunctions_d27);
	bool patternMatcher( const Udm::Object& subsystem_d2b, const Udm::Object& initFunction_d34);
	void effector();

private:
	Packets_t _subsystem_d29;
	Packets_t _initFunction_d32;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_d49;
		SFC::Function initFunction_d4a;
		SFC::LocalVar localVar_d4b;
		SFC::Arg superArg_d4c;
		SFC::Function superInitFunction_d4d;
		CyberComposition::Simulink::Subsystem superSubsystem_d4e;
		SFC::Arg arg_d4f;
	};

	std::list< Match> _matches;
};

class MakeConstructors_d5a
{
public:
	void operator()( const Packets_t& systems_d5b, Packets_t& systems_d5d);

protected:
	void callCreateInitFunction_107f( const Packets_t& subsystems_105b);
	void callinitParameters_1081( const Packets_t& systems_d60, const Packets_t& initFunctions_d62, const Packets_t& classStructs_d64);
	void callinitSubSubsystem_1085( const Packets_t& subsystems_d25, const Packets_t& initFunctions_d27);

private:
	Packets_t* _system_d5e;
};

class InitParameters_d5f
{
public:
	void operator()( const Packets_t& systems_d60, const Packets_t& initFunctions_d62, const Packets_t& classStructs_d64, Packets_t& systems_d66, Packets_t& initFunctions_d67);

protected:
	void callinitDirectParameters_104a( const Packets_t& systems_1006, const Packets_t& initFunctions_1008, const Packets_t& classStructs_100a);
	void callinitPrimitiveParameters_104e( const Packets_t& systems_db7, const Packets_t& initFunctions_db9, const Packets_t& classStructs_dbb);
	void callinitPrimPseudoParams_1052( const Packets_t& systems_d6b, const Packets_t& initFunctions_d6d, const Packets_t& classStructs_d6f);
	void callInitTriggerVars_1056( const Packets_t& systems_f49, const Packets_t& initFunctions_f4b, const Packets_t& classStructs_f4d);

private:
	Packets_t* _system_d68;
	Packets_t* _initFunction_d69;
};

class InitPrimPseudoParams_d6a
{
public:
	void operator()( const Packets_t& systems_d6b, const Packets_t& initFunctions_d6d, const Packets_t& classStructs_d6f, Packets_t& systems_d71, Packets_t& initFunctions_d72, Packets_t& classStructs_d73);

protected:
	void callinitPrimPseudoParam_db2( const Packets_t& subsystem1s_d78, const Packets_t& initFunctions_d7a, const Packets_t& classStructs_d7c);

private:
	Packets_t* _system_d74;
	Packets_t* _initFunction_d75;
	Packets_t* _classStruct_d76;
};

class InitPrimPseudoParam_d77
{
public:
	void operator()( const Packets_t& subsystem1s_d78, const Packets_t& initFunctions_d7a, const Packets_t& classStructs_d7c);

protected:
	bool isInputUnique( const Udm::Object& subsystem1_d82, const Udm::Object& initFunction_d8b, const Udm::Object& classStruct_d94);
	bool isGuardTrue( SFC::Struct& ClassStruct, SFC::Function& InitFunction, SFC::LocalVar& LocalVar, CyberComposition::Simulink::SF_Parameter& Parameter, CyberComposition::Simulink::Primitive& Primitive, CyberComposition::Simulink::Subsystem& Subsystem1, CyberComposition::Simulink::Subsystem& Subsystem2);
	void processInputPackets( const Packets_t& subsystem1s_d78, const Packets_t& initFunctions_d7a, const Packets_t& classStructs_d7c);
	bool patternMatcher( const Udm::Object& subsystem1_d80, const Udm::Object& initFunction_d89, const Udm::Object& classStruct_d92);
	void effector();

private:
	Packets_t _subsystem1_d7e;
	Packets_t _initFunction_d87;
	Packets_t _classStruct_d90;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem1_da4;
		SFC::Function initFunction_da5;
		SFC::Struct classStruct_da6;
		CyberComposition::Simulink::Subsystem subsystem2_da7;
		CyberComposition::Simulink::Primitive primitive_da8;
		CyberComposition::Simulink::SF_Parameter parameter_da9;
		SFC::LocalVar localVar_daa;
	};

	std::list< Match> _matches;
};

class InitPrimitiveParameters_db6
{
public:
	void operator()( const Packets_t& systems_db7, const Packets_t& initFunctions_db9, const Packets_t& classStructs_dbb, Packets_t& systems_dbd, Packets_t& initFunctions_dbe, Packets_t& classStructs_dbf);

protected:
	void callGetStructMembers_f38( const Packets_t& subsystems_ded, const Packets_t& initFunctions_df0, const Packets_t& classStructs_df3);
	void callTest_f3c( const Packets_t& systems_e26, const Packets_t& initFunctions_e28, const Packets_t& members_e2a);
	void callzeroPrimitiveParameter_f40( const Packets_t& subsystems_dc4, const Packets_t& initFunctions_dc6, const Packets_t& localVars_dc8);
	void callinitPrimitiveParameter_f44( const Packets_t& subsystem1s_f03, const Packets_t& initFunctions_f05, const Packets_t& localVars_f07);

private:
	Packets_t* _system_dc0;
	Packets_t* _initFunction_dc1;
	Packets_t* _classStruct_dc2;
};

class ZeroPrimitiveParameter_dc3
{
public:
	void operator()( const Packets_t& subsystems_dc4, const Packets_t& initFunctions_dc6, const Packets_t& localVars_dc8);

protected:
	bool isInputUnique( const Udm::Object& subsystem_dce, const Udm::Object& initFunction_dd7, const Udm::Object& localVar_de0);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_dc4, const Packets_t& initFunctions_dc6, const Packets_t& localVars_dc8);
	bool patternMatcher( const Udm::Object& subsystem_dcc, const Udm::Object& initFunction_dd5, const Udm::Object& localVar_dde);
	void effector();

private:
	Packets_t _subsystem_dca;
	Packets_t _initFunction_dd3;
	Packets_t _localVar_ddc;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_de9;
		SFC::Function initFunction_dea;
		SFC::LocalVar localVar_deb;
	};

	std::list< Match> _matches;
};

class GetStructMembers_dec
{
public:
	void operator()( const Packets_t& subsystems_ded, const Packets_t& initFunctions_df0, const Packets_t& classStructs_df3, Packets_t& subsystems_def, Packets_t& initFunctions_df2, Packets_t& localVars_df5);

protected:
	bool isInputUnique( const Udm::Object& subsystem_dfd, const Udm::Object& initFunction_e06, const Udm::Object& classStruct_e0f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_ded, const Packets_t& initFunctions_df0, const Packets_t& classStructs_df3);
	bool patternMatcher( const Udm::Object& subsystem_dfb, const Udm::Object& initFunction_e04, const Udm::Object& classStruct_e0d);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_e1f, const SFC::Function& initFunction_e21, const SFC::LocalVar& localVar_e23);

private:
	Packets_t* _subsystem_df6;
	Packets_t* _initFunction_df7;
	Packets_t* _localVar_df8;
	Packets_t _subsystem_df9;
	Packets_t _initFunction_e02;
	Packets_t _classStruct_e0b;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_e1b;
		SFC::Function initFunction_e1c;
		SFC::Struct classStruct_e1d;
		SFC::LocalVar localVar_e1e;
	};

	std::list< Match> _matches;
};

class Test_e25
{
public:
	void operator()( const Packets_t& systems_e26, const Packets_t& initFunctions_e28, const Packets_t& members_e2a, Packets_t& systems_e2c, Packets_t& initFunctions_e2d, Packets_t& classStructs_e2e, Packets_t& systems_e2f, Packets_t& initFunctions_e30, Packets_t& classStructs_e31, Packets_t& systems_e32, Packets_t& initFunctions_e33, Packets_t& classStructs_e34);

protected:
	void executeOne( const Packets_t& systems_e26, const Packets_t& initFunctions_e28, const Packets_t& members_e2a);
	bool isInputUnique( const Udm::Object& system_e40, const Udm::Object& initFunction_e47, const Udm::Object& member_e4e);

private:
	Packets_t* _system_e35;
	Packets_t* _initFunction_e36;
	Packets_t* _classStruct_e37;
	Packets_t* _system_e38;
	Packets_t* _initFunction_e39;
	Packets_t* _classStruct_e3a;
	Packets_t* _system_e3b;
	Packets_t* _initFunction_e3c;
	Packets_t* _classStruct_e3d;
	Packets_t _system_e3e;
	Packets_t _initFunction_e45;
	Packets_t _member_e4c;
};

class MemberHasParameter_e53
{
public:
	bool operator()( const Packets_t& subsystem1s_e54, const Packets_t& initFunctions_e57, const Packets_t& localVars_e5a, Packets_t& subsystem1s_e56, Packets_t& initFunctions_e59, Packets_t& localVars_e5c);

protected:
	bool isInputUnique( const Udm::Object& subsystem1_e64, const Udm::Object& initFunction_e6d, const Udm::Object& localVar_e76);
	bool isGuardTrue( SFC::Function& InitFunction, SFC::LocalVar& LocalVar, CyberComposition::Simulink::SF_Parameter& Parameter, CyberComposition::Simulink::Primitive& Primitive, CyberComposition::Simulink::Subsystem& Subsystem1, CyberComposition::Simulink::Subsystem& Subsystem2);
	void processInputPackets( const Packets_t& subsystem1s_e54, const Packets_t& initFunctions_e57, const Packets_t& localVars_e5a);
	bool patternMatcher( const Udm::Object& subsystem1_e62, const Udm::Object& initFunction_e6b, const Udm::Object& localVar_e74);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem1_e8f, const SFC::Function& initFunction_e91, const SFC::LocalVar& localVar_e93);

private:
	Packets_t* _subsystem1_e5d;
	Packets_t* _initFunction_e5e;
	Packets_t* _localVar_e5f;
	Packets_t _subsystem1_e60;
	Packets_t _initFunction_e69;
	Packets_t _localVar_e72;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem1_e83;
		SFC::Function initFunction_e84;
		SFC::LocalVar localVar_e85;
		CyberComposition::Simulink::Subsystem subsystem2_e86;
		CyberComposition::Simulink::Primitive primitive_e87;
		CyberComposition::Simulink::SF_Parameter parameter_e88;
	};

	std::list< Match> _matches;
};

class StructMember_e95
{
public:
	bool operator()( const Packets_t& subsystems_e96, const Packets_t& initFunctions_e99, const Packets_t& localVars_e9c, Packets_t& subsystems_e98, Packets_t& initFunctions_e9b, Packets_t& localVars_e9e);

protected:
	bool isInputUnique( const Udm::Object& subsystem_ea6, const Udm::Object& initFunction_eaf, const Udm::Object& localVar_eb8);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_e96, const Packets_t& initFunctions_e99, const Packets_t& localVars_e9c);
	bool patternMatcher( const Udm::Object& subsystem_ea4, const Udm::Object& initFunction_ead, const Udm::Object& localVar_eb6);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_ec7, const SFC::Function& initFunction_ec9, const SFC::LocalVar& localVar_ecb);

private:
	Packets_t* _subsystem_e9f;
	Packets_t* _initFunction_ea0;
	Packets_t* _localVar_ea1;
	Packets_t _subsystem_ea2;
	Packets_t _initFunction_eab;
	Packets_t _localVar_eb4;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_ec3;
		SFC::Function initFunction_ec4;
		SFC::LocalVar localVar_ec5;
		SFC::Struct struct_ec6;
	};

	std::list< Match> _matches;
};

class MemberNoParameter_ecd
{
public:
	bool operator()( const Packets_t& subsystems_ece, const Packets_t& initFunctions_ed1, const Packets_t& localVars_ed4, Packets_t& subsystems_ed0, Packets_t& initFunctions_ed3, Packets_t& localVars_ed6);

protected:
	bool isInputUnique( const Udm::Object& subsystem_ede, const Udm::Object& initFunction_ee7, const Udm::Object& localVar_ef0);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_ece, const Packets_t& initFunctions_ed1, const Packets_t& localVars_ed4);
	bool patternMatcher( const Udm::Object& subsystem_edc, const Udm::Object& initFunction_ee5, const Udm::Object& localVar_eee);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_efc, const SFC::Function& initFunction_efe, const SFC::LocalVar& localVar_f00);

private:
	Packets_t* _subsystem_ed7;
	Packets_t* _initFunction_ed8;
	Packets_t* _localVar_ed9;
	Packets_t _subsystem_eda;
	Packets_t _initFunction_ee3;
	Packets_t _localVar_eec;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_ef9;
		SFC::Function initFunction_efa;
		SFC::LocalVar localVar_efb;
	};

	std::list< Match> _matches;
};

class InitPrimitiveParameter_f02
{
public:
	void operator()( const Packets_t& subsystem1s_f03, const Packets_t& initFunctions_f05, const Packets_t& localVars_f07);

protected:
	bool isInputUnique( const Udm::Object& subsystem1_f0d, const Udm::Object& initFunction_f16, const Udm::Object& localVar_f1f);
	bool isGuardTrue( SFC::Function& InitFunction, SFC::LocalVar& LocalVar, CyberComposition::Simulink::SF_Parameter& Parameter, CyberComposition::Simulink::Primitive& Primitive, CyberComposition::Simulink::Subsystem& Subsystem1, CyberComposition::Simulink::Subsystem& Subsystem2);
	void processInputPackets( const Packets_t& subsystem1s_f03, const Packets_t& initFunctions_f05, const Packets_t& localVars_f07);
	bool patternMatcher( const Udm::Object& subsystem1_f0b, const Udm::Object& initFunction_f14, const Udm::Object& localVar_f1d);
	void effector();

private:
	Packets_t _subsystem1_f09;
	Packets_t _initFunction_f12;
	Packets_t _localVar_f1b;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem1_f2c;
		SFC::Function initFunction_f2d;
		SFC::LocalVar localVar_f2e;
		CyberComposition::Simulink::Subsystem subsystem2_f2f;
		CyberComposition::Simulink::Primitive primitive_f30;
		CyberComposition::Simulink::SF_Parameter parameter_f31;
	};

	std::list< Match> _matches;
};

class InitTriggerVars_f48
{
public:
	void operator()( const Packets_t& systems_f49, const Packets_t& initFunctions_f4b, const Packets_t& classStructs_f4d);

protected:
	void callInitMatrixTrigger_ffd( const Packets_t& systems_fa4, const Packets_t& initFunctions_fa6, const Packets_t& classStructs_fa8);
	void callInitStructTrigger_1001( const Packets_t& systems_f50, const Packets_t& initFunctions_f52, const Packets_t& classStructs_f54);
};

class InitStructTrigger_f4f
{
public:
	void operator()( const Packets_t& systems_f50, const Packets_t& initFunctions_f52, const Packets_t& classStructs_f54);

protected:
	void callInitStructTrigger_f9f( const Packets_t& subsystems_f57, const Packets_t& initFunctions_f59, const Packets_t& classStructs_f5b);
};

class InitStructTrigger_f56
{
public:
	void operator()( const Packets_t& subsystems_f57, const Packets_t& initFunctions_f59, const Packets_t& classStructs_f5b);

protected:
	bool isInputUnique( const Udm::Object& subsystem_f61, const Udm::Object& initFunction_f6a, const Udm::Object& classStruct_f73);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_f57, const Packets_t& initFunctions_f59, const Packets_t& classStructs_f5b);
	bool patternMatcher( const Udm::Object& subsystem_f5f, const Udm::Object& initFunction_f68, const Udm::Object& classStruct_f71);
	void effector();

private:
	Packets_t _subsystem_f5d;
	Packets_t _initFunction_f66;
	Packets_t _classStruct_f6f;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_f8f;
		SFC::Function initFunction_f90;
		SFC::Struct classStruct_f91;
		CyberComposition::Simulink::TriggerPort triggerPort_f92;
		SFC::Arg arg_f93;
		SFC::LocalVar member_f94;
		SFC::LocalVar localVar_f95;
		SFC::Struct struct_f96;
	};

	std::list< Match> _matches;
};

class InitMatrixTrigger_fa3
{
public:
	void operator()( const Packets_t& systems_fa4, const Packets_t& initFunctions_fa6, const Packets_t& classStructs_fa8, Packets_t& outs_faa, Packets_t& initFunctions_fab, Packets_t& classStructs_fac);

protected:
	void callInitMatrixTrigger_ff9( const Packets_t& subsystems_fb1, const Packets_t& initFunctions_fb3, const Packets_t& classStructs_fb5);

private:
	Packets_t* _out_fad;
	Packets_t* _initFunction_fae;
	Packets_t* _classStruct_faf;
};

class InitMatrixTrigger_fb0
{
public:
	void operator()( const Packets_t& subsystems_fb1, const Packets_t& initFunctions_fb3, const Packets_t& classStructs_fb5);

protected:
	bool isInputUnique( const Udm::Object& subsystem_fbb, const Udm::Object& initFunction_fc4, const Udm::Object& classStruct_fcd);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg, SFC::Struct& ClassStruct, SFC::DT& DT, SFC::Function& InitFunction, SFC::LocalVar& LocalVar, CyberComposition::Simulink::Subsystem& Subsystem, CyberComposition::Simulink::TriggerPort& TriggerPort);
	void processInputPackets( const Packets_t& subsystems_fb1, const Packets_t& initFunctions_fb3, const Packets_t& classStructs_fb5);
	bool patternMatcher( const Udm::Object& subsystem_fb9, const Udm::Object& initFunction_fc2, const Udm::Object& classStruct_fcb);
	void effector();

private:
	Packets_t _subsystem_fb7;
	Packets_t _initFunction_fc0;
	Packets_t _classStruct_fc9;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_fe5;
		SFC::Function initFunction_fe6;
		SFC::Struct classStruct_fe7;
		CyberComposition::Simulink::TriggerPort triggerPort_fe8;
		SFC::Arg arg_fe9;
		SFC::LocalVar localVar_fea;
		SFC::DT dT_feb;
	};

	std::list< Match> _matches;
};

class InitDirectParameters_1005
{
public:
	void operator()( const Packets_t& systems_1006, const Packets_t& initFunctions_1008, const Packets_t& classStructs_100a, Packets_t& systems_100c, Packets_t& initFunctions_100d, Packets_t& classStructs_100e);

protected:
	void callinitDirectParameter_1046( const Packets_t& subsystems_1013, const Packets_t& initFunctions_1015, const Packets_t& classStructs_1017);

private:
	Packets_t* _system_100f;
	Packets_t* _initFunction_1010;
	Packets_t* _classStruct_1011;
};

class InitDirectParameter_1012
{
public:
	void operator()( const Packets_t& subsystems_1013, const Packets_t& initFunctions_1015, const Packets_t& classStructs_1017);

protected:
	bool isInputUnique( const Udm::Object& subsystem_101d, const Udm::Object& initFunction_1026, const Udm::Object& classStruct_102f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1013, const Packets_t& initFunctions_1015, const Packets_t& classStructs_1017);
	bool patternMatcher( const Udm::Object& subsystem_101b, const Udm::Object& initFunction_1024, const Udm::Object& classStruct_102d);
	void effector();

private:
	Packets_t _subsystem_1019;
	Packets_t _initFunction_1022;
	Packets_t _classStruct_102b;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1041;
		SFC::Function initFunction_1042;
		SFC::Struct classStruct_1043;
		CyberComposition::Simulink::SF_Parameter parameter_1044;
		SFC::LocalVar localVar_1045;
	};

	std::list< Match> _matches;
};

class CreateInitFunction_105a
{
public:
	void operator()( const Packets_t& subsystems_105b, Packets_t& subsystems_105d, Packets_t& initFunctions_105e, Packets_t& structs_105f);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1067);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_105b);
	bool patternMatcher( const Udm::Object& subsystem_1065);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_1079, const SFC::Function& initFunction_107b, const SFC::Struct& struct_107d);

private:
	Packets_t* _subsystem_1060;
	Packets_t* _initFunction_1061;
	Packets_t* _struct_1062;
	Packets_t _subsystem_1063;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1074;
		SFC::Class class_1075;
		SFC::Struct struct_1076;
	};

	std::list< Match> _matches;
};

class MakeSFChartConstructor_1088
{
public:
	void operator()( const Packets_t& systems_1089);

protected:
	void callCreateInitFunction_10ac( const Packets_t& subsystems_108c);
	void callinitSubSubsystem_10ae( const Packets_t& subsystems_d25, const Packets_t& initFunctions_d27);
};

class CreateInitFunction_108b
{
public:
	void operator()( const Packets_t& subsystems_108c, Packets_t& subsystems_108e, Packets_t& initFunctions_108f);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1096);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_108c);
	bool patternMatcher( const Udm::Object& subsystem_1094);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_10a8, const SFC::Function& initFunction_10aa);

private:
	Packets_t* _subsystem_1090;
	Packets_t* _initFunction_1091;
	Packets_t _subsystem_1092;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_10a3;
		SFC::Class class_10a4;
		SFC::Struct struct_10a5;
	};

	std::list< Match> _matches;
};

class GetSubSubsystems_10b1
{
public:
	void operator()( const Packets_t& systems_10b2, Packets_t& subsystems_10b4);

protected:
	void callGetSubSubsystems_10cf( const Packets_t& subsystems_10b7);

private:
	Packets_t* _subsystem_10b5;
};

class GetSubSubsystems_10b6
{
public:
	void operator()( const Packets_t& subsystems_10b7, Packets_t& subSubsystems_10b9);

protected:
	bool isInputUnique( const Udm::Object& subsystem_10bf);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_10b7);
	bool patternMatcher( const Udm::Object& subsystem_10bd);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subSubsystem_10cd);

private:
	Packets_t* _subSubsystem_10ba;
	Packets_t _subsystem_10bb;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_10cb;
		CyberComposition::Simulink::Subsystem subSubsystem_10cc;
	};

	std::list< Match> _matches;
};

class SubsystemFilter_10d1
{
public:
	void operator()( const Packets_t& systems_10d2, Packets_t& chartsystems_10d4, Packets_t& sFchartsystems_10d5, Packets_t& systems_10d6);

protected:
	void callSubsystemFilter_114d( const Packets_t& systems_10db);

private:
	Packets_t* _chartsystem_10d7;
	Packets_t* _sFchartsystem_10d8;
	Packets_t* _system_10d9;
};

class SubsystemFilter_10da
{
public:
	void operator()( const Packets_t& systems_10db, Packets_t& systems_10dd, Packets_t& systems_10de, Packets_t& systems_10df, Packets_t& systems_10e0);

protected:
	void executeOne( const Packets_t& systems_10db);
	bool isInputUnique( const Udm::Object& system_10e7);

private:
	Packets_t* _system_10e1;
	Packets_t* _system_10e2;
	Packets_t* _system_10e3;
	Packets_t* _system_10e4;
	Packets_t _system_10e5;
};

class IsStateChart_10ec
{
public:
	bool operator()( const Packets_t& subsystems_10ed, Packets_t& subsystems_10ef);

protected:
	bool isInputUnique( const Udm::Object& subsystem_10f5);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_10ed);
	bool patternMatcher( const Udm::Object& subsystem_10f3);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_1107);

private:
	Packets_t* _subsystem_10f0;
	Packets_t _subsystem_10f1;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1104;
		CyberComposition::Simulink::Primitive primitive_1105;
		CyberComposition::Simulink::State state_1106;
	};

	std::list< Match> _matches;
};

class IsSFStateChart_1109
{
public:
	bool operator()( const Packets_t& subsystems_110a, Packets_t& subsystems_110c);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1112);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_110a);
	bool patternMatcher( const Udm::Object& subsystem_1110);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_1120);

private:
	Packets_t* _subsystem_110d;
	Packets_t _subsystem_110e;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_111e;
		CyberComposition::Simulink::SFState sFState_111f;
	};

	std::list< Match> _matches;
};

class IsInstance_1122
{
public:
	bool operator()( const Packets_t& subsystems_1123, Packets_t& subsystems_1125);

protected:
	bool isInputUnique( const Udm::Object& subsystem_112b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& subsystems_1123);
	bool patternMatcher( const Udm::Object& subsystem_1129);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_1136);

private:
	Packets_t* _subsystem_1126;
	Packets_t _subsystem_1127;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1134;
	};

	std::list< Match> _matches;
};

class Otherwise_1138
{
public:
	bool operator()( const Packets_t& subsystems_1139, Packets_t& subsystems_113b);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1141);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1139);
	bool patternMatcher( const Udm::Object& subsystem_113f);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_114b);

private:
	Packets_t* _subsystem_113c;
	Packets_t _subsystem_113d;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_114a;
	};

	std::list< Match> _matches;
};

class GetSubsystems_1159
{
public:
	void operator()( const Packets_t& comptypes_115a, Packets_t& subsystems_115c);

protected:
	void callGetSubSystems_1177( const Packets_t& dataflows_115f);

private:
	Packets_t* _subsystem_115d;
};

class GetSubSystems_115e
{
public:
	void operator()( const Packets_t& dataflows_115f, Packets_t& subsystems_1161);

protected:
	bool isInputUnique( const Udm::Object& dataflow_1167);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& dataflows_115f);
	bool patternMatcher( const Udm::Object& dataflow_1165);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_1175);

private:
	Packets_t* _subsystem_1162;
	Packets_t _dataflow_1163;
	class Match
	{
	public:
		CyberComposition::SimulinkWrapper dataflow_1173;
		CyberComposition::Simulink::Subsystem subsystem_1174;
	};

	std::list< Match> _matches;
};

class MakeClasses_117d
{
public:
	void operator()( const Packets_t& dataflows_117e, const Packets_t& projects_1180, Packets_t& dataflows_1182, Packets_t& projects_1183);

protected:
	void callGetBlocks_5323( const Packets_t& comptypes_52ef, const Packets_t& projects_52f1);
	void callCreateAllPrograms_5326( const Packets_t& subsystems_1187, const Packets_t& projects_1189);

private:
	Packets_t* _dataflow_1184;
	Packets_t* _project_1185;
};

class CreateAllPrograms_1186
{
public:
	void operator()( const Packets_t& subsystems_1187, const Packets_t& projects_1189);

protected:
	void callCreateProgramsTopLevel_52e2( const Packets_t& systems_1c43, const Packets_t& projects_1c45);
	void callCreateClasses_52e5( const Packets_t& systems_1739, const Packets_t& programs_173b);
	void callCreateFunctionsAndArgs_52e8( const Packets_t& systems_118c);
	void callCreateFunctionLocalVars_52ea( const Packets_t& systems_18ec);
	void callCreateSigFlowsR_52ec( const Packets_t& systems_1cef);
};

class CreateFunctionsAndArgs_118b
{
public:
	void operator()( const Packets_t& systems_118c, Packets_t& systems_118e);

protected:
	void callSubsystemFilter_1725( const Packets_t& systems_10d2);
	void callCreateChartFunction_1727( const Packets_t& subsystems_12dc);
	void callCreateFunctions_1729( const Packets_t& subsystems_1596);
	void callCreateSFChartFunction_172b( const Packets_t& subsystems_1702);
	void callMakeChartArguments_172d( const Packets_t& states_1191, const Packets_t& functions_1193);
	void callMakeArguments_1730( const Packets_t& systems_1304, const Packets_t& functions_1306);
	void callMakeSFChartArguments_1733( const Packets_t& states_15b7, const Packets_t& functions_15b9);
	void callGetSubSubsystems_1736( const Packets_t& systems_10b2);

private:
	Packets_t* _system_118f;
};

class MakeChartArguments_1190
{
public:
	void operator()( const Packets_t& states_1191, const Packets_t& functions_1193);

protected:
	void callDataInput_12ce( const Packets_t& states_1288, const Packets_t& functions_128a);
	void callEventInput_12d1( const Packets_t& states_1241, const Packets_t& functions_1243);
	void callDataOutput_12d4( const Packets_t& states_11fa, const Packets_t& functions_11fc);
	void callEventOutput_12d7( const Packets_t& states_1196, const Packets_t& functions_1198);
};

class EventOutput_1195
{
public:
	void operator()( const Packets_t& states_1196, const Packets_t& functions_1198);

protected:
	void callEventOutput_11f3( const Packets_t& states_119b, const Packets_t& mains_119e);
	void callUpdateArgCount_11f6( const Packets_t& args_11d4, const Packets_t& mains_11d6);
};

class EventOutput_119a
{
public:
	void operator()( const Packets_t& states_119b, const Packets_t& mains_119e, Packets_t& args_119d, Packets_t& mains_11a0);

protected:
	bool isInputUnique( const Udm::Object& state_11a7, const Udm::Object& main_11b0);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::DT& DT, CyberComposition::Simulink::Event& Event, SFC::Function& Main, CyberComposition::Simulink::State& State, CyberComposition::Simulink::TypeBaseRef& TypeBaseRef, CyberComposition::Simulink::TypeStruct& TypeStruct);
	void processInputPackets( const Packets_t& states_119b, const Packets_t& mains_119e);
	bool patternMatcher( const Udm::Object& state_11a5, const Udm::Object& main_11ae);
	void effector();
	void outputAppender( const SFC::Arg& arg_11cf, const SFC::Function& main_11d1);

private:
	Packets_t* _arg_11a1;
	Packets_t* _main_11a2;
	Packets_t _state_11a3;
	Packets_t _main_11ac;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_11c2;
		SFC::Function main_11c3;
		CyberComposition::Simulink::TypeStruct typeStruct_11c4;
		SFC::DT dT_11c5;
		CyberComposition::Simulink::Event event_11c6;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_11c7;
	};

	std::list< Match> _matches;
};

class UpdateArgCount_11d3
{
public:
	void operator()( const Packets_t& args_11d4, const Packets_t& mains_11d6);

protected:
	bool isInputUnique( const Udm::Object& arg_11dc, const Udm::Object& main_11e5);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& args_11d4, const Packets_t& mains_11d6);
	bool patternMatcher( const Udm::Object& arg_11da, const Udm::Object& main_11e3);
	void effector();

private:
	Packets_t _arg_11d8;
	Packets_t _main_11e1;
	class Match
	{
	public:
		SFC::Arg arg_11f1;
		SFC::Function main_11f2;
	};

	std::list< Match> _matches;
};

class DataOutput_11f9
{
public:
	void operator()( const Packets_t& states_11fa, const Packets_t& functions_11fc, Packets_t& states_11fe, Packets_t& functions_11ff);

protected:
	void callDataOutput_123a( const Packets_t& states_1203, const Packets_t& mains_1206);
	void callUpdateArgCount_123d( const Packets_t& args_11d4, const Packets_t& mains_11d6);

private:
	Packets_t* _state_1200;
	Packets_t* _function_1201;
};

class DataOutput_1202
{
public:
	void operator()( const Packets_t& states_1203, const Packets_t& mains_1206, Packets_t& args_1205, Packets_t& mains_1208);

protected:
	bool isInputUnique( const Udm::Object& state_120f, const Udm::Object& main_1218);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::DT& DT, CyberComposition::Simulink::Data& Data, SFC::Function& Main, CyberComposition::Simulink::State& State, CyberComposition::Simulink::SF_TypeBase& TypeBase, CyberComposition::Simulink::TypeBaseRef& TypeBaseRef);
	void processInputPackets( const Packets_t& states_1203, const Packets_t& mains_1206);
	bool patternMatcher( const Udm::Object& state_120d, const Udm::Object& main_1216);
	void effector();
	void outputAppender( const SFC::Arg& arg_1236, const SFC::Function& main_1238);

private:
	Packets_t* _arg_1209;
	Packets_t* _main_120a;
	Packets_t _state_120b;
	Packets_t _main_1214;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1229;
		SFC::Function main_122a;
		CyberComposition::Simulink::SF_TypeBase typeBase_122b;
		SFC::DT dT_122c;
		CyberComposition::Simulink::Data data_122d;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_122e;
	};

	std::list< Match> _matches;
};

class EventInput_1240
{
public:
	void operator()( const Packets_t& states_1241, const Packets_t& functions_1243, Packets_t& states_1245, Packets_t& functions_1246);

protected:
	void callEventInput_1281( const Packets_t& states_124a, const Packets_t& mains_124d);
	void callUpdateArgCount_1284( const Packets_t& args_11d4, const Packets_t& mains_11d6);

private:
	Packets_t* _state_1247;
	Packets_t* _function_1248;
};

class EventInput_1249
{
public:
	void operator()( const Packets_t& states_124a, const Packets_t& mains_124d, Packets_t& args_124c, Packets_t& mains_124f);

protected:
	bool isInputUnique( const Udm::Object& state_1256, const Udm::Object& main_125f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::DT& DT, CyberComposition::Simulink::Event& Event, SFC::Function& Main, CyberComposition::Simulink::State& State, CyberComposition::Simulink::SF_TypeBase& TypeBase, CyberComposition::Simulink::TypeBaseRef& TypeBaseRef);
	void processInputPackets( const Packets_t& states_124a, const Packets_t& mains_124d);
	bool patternMatcher( const Udm::Object& state_1254, const Udm::Object& main_125d);
	void effector();
	void outputAppender( const SFC::Arg& arg_127d, const SFC::Function& main_127f);

private:
	Packets_t* _arg_1250;
	Packets_t* _main_1251;
	Packets_t _state_1252;
	Packets_t _main_125b;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1270;
		SFC::Function main_1271;
		CyberComposition::Simulink::SF_TypeBase typeBase_1272;
		SFC::DT dT_1273;
		CyberComposition::Simulink::Event event_1274;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_1275;
	};

	std::list< Match> _matches;
};

class DataInput_1287
{
public:
	void operator()( const Packets_t& states_1288, const Packets_t& functions_128a, Packets_t& states_128c, Packets_t& functions_128d);

protected:
	void callDataInput_12c8( const Packets_t& states_1291, const Packets_t& mains_1294);
	void callUpdateArgCount_12cb( const Packets_t& args_11d4, const Packets_t& mains_11d6);

private:
	Packets_t* _state_128e;
	Packets_t* _function_128f;
};

class DataInput_1290
{
public:
	void operator()( const Packets_t& states_1291, const Packets_t& mains_1294, Packets_t& args_1293, Packets_t& mains_1296);

protected:
	bool isInputUnique( const Udm::Object& state_129d, const Udm::Object& main_12a6);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::DT& DT, CyberComposition::Simulink::Data& Data, SFC::Function& Main, CyberComposition::Simulink::State& State, CyberComposition::Simulink::SF_TypeBase& TypeBase, CyberComposition::Simulink::TypeBaseRef& TypeBaseRef);
	void processInputPackets( const Packets_t& states_1291, const Packets_t& mains_1294);
	bool patternMatcher( const Udm::Object& state_129b, const Udm::Object& main_12a4);
	void effector();
	void outputAppender( const SFC::Arg& arg_12c4, const SFC::Function& main_12c6);

private:
	Packets_t* _arg_1297;
	Packets_t* _main_1298;
	Packets_t _state_1299;
	Packets_t _main_12a2;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_12b7;
		SFC::Function main_12b8;
		CyberComposition::Simulink::SF_TypeBase typeBase_12b9;
		SFC::DT dT_12ba;
		CyberComposition::Simulink::Data data_12bb;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_12bc;
	};

	std::list< Match> _matches;
};

class CreateChartFunction_12da
{
public:
	void operator()( const Packets_t& subsystems_12dc, Packets_t& states_12db, Packets_t& mains_12de);

protected:
	bool isInputUnique( const Udm::Object& subsystem_12e5);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_12dc);
	bool patternMatcher( const Udm::Object& subsystem_12e3);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_12ff, const SFC::Function& main_1301);

private:
	Packets_t* _state_12df;
	Packets_t* _main_12e0;
	Packets_t _subsystem_12e1;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_12f8;
		CyberComposition::Simulink::Primitive primitive_12f9;
		CyberComposition::Simulink::State state_12fa;
		SFC::Class class_12fb;
		SFC::Struct struct_12fc;
	};

	std::list< Match> _matches;
};

class MakeArguments_1303
{
public:
	void operator()( const Packets_t& systems_1304, const Packets_t& functions_1306, Packets_t& systems_1308);

protected:
	void callTrigs_1586( const Packets_t& systems_1466, const Packets_t& functions_1468);
	void callActions_1589( const Packets_t& systems_130b, const Packets_t& functions_130d);
	void callInputs_158c( const Packets_t& systems_149f, const Packets_t& functions_14a1);
	void callOutputs_158f( const Packets_t& systems_1513, const Packets_t& functions_1515);
	void callZeroUnconOutputs_1592( const Packets_t& systems_1371, const Packets_t& functions_1373);

private:
	Packets_t* _system_1309;
};

class Actions_130a
{
public:
	void operator()( const Packets_t& systems_130b, const Packets_t& functions_130d, Packets_t& systems_130f, Packets_t& functions_1310);

protected:
	void callCreateTriggerPortArgs_136a( const Packets_t& subsystems_1314, const Packets_t& mains_1318);
	void callAssignType_136d( const Packets_t& ports_133e, const Packets_t& argvars_1340);

private:
	Packets_t* _system_1311;
	Packets_t* _function_1312;
};

class CreateTriggerPortArgs_1313
{
public:
	void operator()( const Packets_t& subsystems_1314, const Packets_t& mains_1318, Packets_t& actionPorts_1316, Packets_t& args_1317);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1320, const Udm::Object& main_1329);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1314, const Packets_t& mains_1318);
	bool patternMatcher( const Udm::Object& subsystem_131e, const Udm::Object& main_1327);
	void effector();
	void outputAppender( const CyberComposition::Simulink::EnablePort& actionPort_1339, const SFC::Arg& arg_133b);

private:
	Packets_t* _actionPort_131a;
	Packets_t* _arg_131b;
	Packets_t _subsystem_131c;
	Packets_t _main_1325;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1335;
		SFC::Function main_1336;
		CyberComposition::Simulink::EnablePort actionPort_1337;
	};

	std::list< Match> _matches;
};

class AssignType_133d
{
public:
	void operator()( const Packets_t& ports_133e, const Packets_t& argvars_1340);

protected:
	void callAssignType_1367( const Packets_t& ports_1343, const Packets_t& argvars_1345);
};

class AssignType_1342
{
public:
	void operator()( const Packets_t& ports_1343, const Packets_t& argvars_1345);

protected:
	bool isInputUnique( const Udm::Object& port_134b, const Udm::Object& argvar_1354);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& ports_1343, const Packets_t& argvars_1345);
	bool patternMatcher( const Udm::Object& port_1349, const Udm::Object& argvar_1352);
	void effector();

private:
	Packets_t _port_1347;
	Packets_t _argvar_1350;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Port port_1362;
		SFC::ArgDeclBase argvar_1363;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_1364;
		CyberComposition::Simulink::SF_TypeBase typeBase_1365;
		SFC::DT dT_1366;
	};

	std::list< Match> _matches;
};

class ZeroUnconOutputs_1370
{
public:
	void operator()( const Packets_t& systems_1371, const Packets_t& functions_1373, Packets_t& systems_1375);

protected:
	void callGetOutputPorts_145c( const Packets_t& subsystems_1399, const Packets_t& mains_139c);
	void callOutPortConnected_145f( const Packets_t& outps_13c2, const Packets_t& functions_13c4);
	void callZeroOutPort_1462( const Packets_t& outPorts_1378, const Packets_t& mains_137a);

private:
	Packets_t* _system_1376;
};

class ZeroOutPort_1377
{
public:
	void operator()( const Packets_t& outPorts_1378, const Packets_t& mains_137a);

protected:
	bool isInputUnique( const Udm::Object& outPort_1380, const Udm::Object& main_1389);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& outPorts_1378, const Packets_t& mains_137a);
	bool patternMatcher( const Udm::Object& outPort_137e, const Udm::Object& main_1387);
	void effector();

private:
	Packets_t _outPort_137c;
	Packets_t _main_1385;
	class Match
	{
	public:
		CyberComposition::Simulink::OutPort outPort_1395;
		SFC::Function main_1396;
		SFC::Arg arg_1397;
	};

	std::list< Match> _matches;
};

class GetOutputPorts_1398
{
public:
	void operator()( const Packets_t& subsystems_1399, const Packets_t& mains_139c, Packets_t& outPorts_139b, Packets_t& mains_139e);

protected:
	bool isInputUnique( const Udm::Object& subsystem_13a5, const Udm::Object& main_13ae);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1399, const Packets_t& mains_139c);
	bool patternMatcher( const Udm::Object& subsystem_13a3, const Udm::Object& main_13ac);
	void effector();
	void outputAppender( const CyberComposition::Simulink::OutPort& outPort_13bd, const SFC::Function& main_13bf);

private:
	Packets_t* _outPort_139f;
	Packets_t* _main_13a0;
	Packets_t _subsystem_13a1;
	Packets_t _main_13aa;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_13ba;
		SFC::Function main_13bb;
		CyberComposition::Simulink::OutPort outPort_13bc;
	};

	std::list< Match> _matches;
};

class OutPortConnected_13c1
{
public:
	void operator()( const Packets_t& outps_13c2, const Packets_t& functions_13c4, Packets_t& outps_13c6, Packets_t& functions_13c7, Packets_t& outps_13c8, Packets_t& functions_13c9, Packets_t& outps_13ca, Packets_t& functions_13cb);

protected:
	void executeOne( const Packets_t& outps_13c2, const Packets_t& functions_13c4);
	bool isInputUnique( const Udm::Object& outp_13d4, const Udm::Object& function_13db);

private:
	Packets_t* _outp_13cc;
	Packets_t* _function_13cd;
	Packets_t* _outp_13ce;
	Packets_t* _function_13cf;
	Packets_t* _outp_13d0;
	Packets_t* _function_13d1;
	Packets_t _outp_13d2;
	Packets_t _function_13d9;
};

class Connected_line_13e0
{
public:
	bool operator()( const Packets_t& outPorts_13e1, const Packets_t& mains_13e4, Packets_t& outPorts_13e3, Packets_t& mains_13e6);

protected:
	bool isInputUnique( const Udm::Object& outPort_13ed, const Udm::Object& main_13f6);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& outPorts_13e1, const Packets_t& mains_13e4);
	bool patternMatcher( const Udm::Object& outPort_13eb, const Udm::Object& main_13f4);
	void outputAppender( const CyberComposition::Simulink::OutPort& outPort_1407, const SFC::Function& main_1409);

private:
	Packets_t* _outPort_13e7;
	Packets_t* _main_13e8;
	Packets_t _outPort_13e9;
	Packets_t _main_13f2;
	class Match
	{
	public:
		CyberComposition::Simulink::OutPort outPort_1403;
		SFC::Function main_1404;
		CyberComposition::Simulink::Line line_1405;
		CyberComposition::Simulink::SF_Port port_1406;
	};

	std::list< Match> _matches;
};

class Connected_ConnToSFStates_140b
{
public:
	bool operator()( const Packets_t& outPorts_140c, const Packets_t& mains_140f, Packets_t& outPorts_140e, Packets_t& mains_1411);

protected:
	bool isInputUnique( const Udm::Object& outPort_1418, const Udm::Object& main_1421);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& outPorts_140c, const Packets_t& mains_140f);
	bool patternMatcher( const Udm::Object& outPort_1416, const Udm::Object& main_141f);
	void outputAppender( const CyberComposition::Simulink::OutPort& outPort_1433, const SFC::Function& main_1435);

private:
	Packets_t* _outPort_1412;
	Packets_t* _main_1413;
	Packets_t _outPort_1414;
	Packets_t _main_141d;
	class Match
	{
	public:
		CyberComposition::Simulink::OutPort outPort_142f;
		SFC::Function main_1430;
		CyberComposition::Simulink::ConnToSFStates connToSFStates_1431;
		CyberComposition::Simulink::SFStateDE sFStateDE_1432;
	};

	std::list< Match> _matches;
};

class Otherwise_1437
{
public:
	bool operator()( const Packets_t& outPorts_1438, const Packets_t& mains_143b, Packets_t& outPorts_143a, Packets_t& mains_143d);

protected:
	bool isInputUnique( const Udm::Object& outPort_1444, const Udm::Object& main_144d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& outPorts_1438, const Packets_t& mains_143b);
	bool patternMatcher( const Udm::Object& outPort_1442, const Udm::Object& main_144b);
	void outputAppender( const CyberComposition::Simulink::OutPort& outPort_1458, const SFC::Function& main_145a);

private:
	Packets_t* _outPort_143e;
	Packets_t* _main_143f;
	Packets_t _outPort_1440;
	Packets_t _main_1449;
	class Match
	{
	public:
		CyberComposition::Simulink::OutPort outPort_1456;
		SFC::Function main_1457;
	};

	std::list< Match> _matches;
};

class Trigs_1465
{
public:
	void operator()( const Packets_t& systems_1466, const Packets_t& functions_1468, Packets_t& systems_146a, Packets_t& functions_146b);

protected:
	void callCreateTriggerPortArgs_1498( const Packets_t& subsystems_146f, const Packets_t& mains_1473);
	void callAssignType_149b( const Packets_t& ports_133e, const Packets_t& argvars_1340);

private:
	Packets_t* _system_146c;
	Packets_t* _function_146d;
};

class CreateTriggerPortArgs_146e
{
public:
	void operator()( const Packets_t& subsystems_146f, const Packets_t& mains_1473, Packets_t& triggerPorts_1471, Packets_t& args_1472);

protected:
	bool isInputUnique( const Udm::Object& subsystem_147b, const Udm::Object& main_1484);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_146f, const Packets_t& mains_1473);
	bool patternMatcher( const Udm::Object& subsystem_1479, const Udm::Object& main_1482);
	void effector();
	void outputAppender( const CyberComposition::Simulink::TriggerPort& triggerPort_1494, const SFC::Arg& arg_1496);

private:
	Packets_t* _triggerPort_1475;
	Packets_t* _arg_1476;
	Packets_t _subsystem_1477;
	Packets_t _main_1480;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1490;
		SFC::Function main_1491;
		CyberComposition::Simulink::TriggerPort triggerPort_1492;
	};

	std::list< Match> _matches;
};

class Inputs_149e
{
public:
	void operator()( const Packets_t& systems_149f, const Packets_t& functions_14a1, Packets_t& systems_14a3, Packets_t& functions_14a4);

protected:
	void callCreateInputPortArgs_1508( const Packets_t& subsystems_14a9, const Packets_t& mains_14ac);
	void callUpdateArgCount_150b( const Packets_t& inputPorts_14d6, const Packets_t& args_14d9, const Packets_t& mains_14dc);
	void callAssignType_150f( const Packets_t& ports_133e, const Packets_t& argvars_1340);

private:
	Packets_t* _system_14a5;
	Packets_t* _function_14a6;
};

class CreateInputPortArgs_14a7
{
public:
	void operator()( const Packets_t& subsystems_14a9, const Packets_t& mains_14ac, Packets_t& inputPorts_14a8, Packets_t& args_14ab, Packets_t& mains_14ae);

protected:
	bool isInputUnique( const Udm::Object& subsystem_14b6, const Udm::Object& main_14bf);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_14a9, const Packets_t& mains_14ac);
	bool patternMatcher( const Udm::Object& subsystem_14b4, const Udm::Object& main_14bd);
	void effector();
	void outputAppender( const CyberComposition::Simulink::InputPort& inputPort_14cf, const SFC::Arg& arg_14d1, const SFC::Function& main_14d3);

private:
	Packets_t* _inputPort_14af;
	Packets_t* _arg_14b0;
	Packets_t* _main_14b1;
	Packets_t _subsystem_14b2;
	Packets_t _main_14bb;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_14cb;
		SFC::Function main_14cc;
		CyberComposition::Simulink::InputPort inputPort_14cd;
	};

	std::list< Match> _matches;
};

class UpdateArgCount_14d5
{
public:
	void operator()( const Packets_t& inputPorts_14d6, const Packets_t& args_14d9, const Packets_t& mains_14dc, Packets_t& inputPorts_14d8, Packets_t& args_14db);

protected:
	bool isInputUnique( const Udm::Object& inputPort_14e4, const Udm::Object& arg_14ed, const Udm::Object& main_14f6);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& inputPorts_14d6, const Packets_t& args_14d9, const Packets_t& mains_14dc);
	bool patternMatcher( const Udm::Object& inputPort_14e2, const Udm::Object& arg_14eb, const Udm::Object& main_14f4);
	void effector();
	void forwardInputs();

private:
	Packets_t* _inputPort_14de;
	Packets_t* _arg_14df;
	Packets_t _inputPort_14e0;
	Packets_t _arg_14e9;
	Packets_t _main_14f2;
	class Match
	{
	public:
		CyberComposition::Simulink::InputPort inputPort_1505;
		SFC::Arg arg_1506;
		SFC::Function main_1507;
	};

	std::list< Match> _matches;
};

class Outputs_1512
{
public:
	void operator()( const Packets_t& systems_1513, const Packets_t& functions_1515, Packets_t& systems_1517, Packets_t& functions_1518);

protected:
	void callCreateOutputPortArgs_157c( const Packets_t& subsystems_151d, const Packets_t& mains_1520);
	void callMainArgCount_157f( const Packets_t& outputPorts_154a, const Packets_t& args_154d, const Packets_t& mains_1550);
	void callAssignType_1583( const Packets_t& ports_133e, const Packets_t& argvars_1340);

private:
	Packets_t* _system_1519;
	Packets_t* _function_151a;
};

class CreateOutputPortArgs_151b
{
public:
	void operator()( const Packets_t& subsystems_151d, const Packets_t& mains_1520, Packets_t& outputPorts_151c, Packets_t& args_151f, Packets_t& mains_1522);

protected:
	bool isInputUnique( const Udm::Object& subsystem_152a, const Udm::Object& main_1533);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_151d, const Packets_t& mains_1520);
	bool patternMatcher( const Udm::Object& subsystem_1528, const Udm::Object& main_1531);
	void effector();
	void outputAppender( const CyberComposition::Simulink::OutputPort& outputPort_1543, const SFC::Arg& arg_1545, const SFC::Function& main_1547);

private:
	Packets_t* _outputPort_1523;
	Packets_t* _arg_1524;
	Packets_t* _main_1525;
	Packets_t _subsystem_1526;
	Packets_t _main_152f;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_153f;
		SFC::Function main_1540;
		CyberComposition::Simulink::OutputPort outputPort_1541;
	};

	std::list< Match> _matches;
};

class MainArgCount_1549
{
public:
	void operator()( const Packets_t& outputPorts_154a, const Packets_t& args_154d, const Packets_t& mains_1550, Packets_t& outputPorts_154c, Packets_t& args_154f);

protected:
	bool isInputUnique( const Udm::Object& outputPort_1558, const Udm::Object& arg_1561, const Udm::Object& main_156a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& outputPorts_154a, const Packets_t& args_154d, const Packets_t& mains_1550);
	bool patternMatcher( const Udm::Object& outputPort_1556, const Udm::Object& arg_155f, const Udm::Object& main_1568);
	void effector();
	void forwardInputs();

private:
	Packets_t* _outputPort_1552;
	Packets_t* _arg_1553;
	Packets_t _outputPort_1554;
	Packets_t _arg_155d;
	Packets_t _main_1566;
	class Match
	{
	public:
		CyberComposition::Simulink::OutputPort outputPort_1579;
		SFC::Arg arg_157a;
		SFC::Function main_157b;
	};

	std::list< Match> _matches;
};

class CreateFunctions_1595
{
public:
	void operator()( const Packets_t& subsystems_1596, Packets_t& subsystems_1598, Packets_t& mains_1599);

protected:
	bool isInputUnique( const Udm::Object& subsystem_15a0);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1596);
	bool patternMatcher( const Udm::Object& subsystem_159e);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_15b2, const SFC::Function& main_15b4);

private:
	Packets_t* _subsystem_159a;
	Packets_t* _main_159b;
	Packets_t _subsystem_159c;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_15ad;
		SFC::Class class_15ae;
		SFC::Struct struct_15af;
	};

	std::list< Match> _matches;
};

class MakeSFChartArguments_15b6
{
public:
	void operator()( const Packets_t& states_15b7, const Packets_t& functions_15b9);

protected:
	void callDataInput_16f4( const Packets_t& states_15bc, const Packets_t& functions_15be);
	void callEventInput_16f7( const Packets_t& states_1623, const Packets_t& functions_1625);
	void callDataOutput_16fa( const Packets_t& states_166a, const Packets_t& functions_166c);
	void callEventOutput_16fd( const Packets_t& states_16b1, const Packets_t& functions_16b3);
};

class DataInput_15bb
{
public:
	void operator()( const Packets_t& states_15bc, const Packets_t& functions_15be, Packets_t& states_15c0, Packets_t& functions_15c1);

protected:
	void callDataInput_161c( const Packets_t& states_15e5, const Packets_t& mains_15e8);
	void callUpdateArgCount_161f( const Packets_t& args_15c5, const Packets_t& mains_15c7);

private:
	Packets_t* _state_15c2;
	Packets_t* _function_15c3;
};

class UpdateArgCount_15c4
{
public:
	void operator()( const Packets_t& args_15c5, const Packets_t& mains_15c7);

protected:
	bool isInputUnique( const Udm::Object& arg_15cd, const Udm::Object& main_15d6);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& args_15c5, const Packets_t& mains_15c7);
	bool patternMatcher( const Udm::Object& arg_15cb, const Udm::Object& main_15d4);
	void effector();

private:
	Packets_t _arg_15c9;
	Packets_t _main_15d2;
	class Match
	{
	public:
		SFC::Arg arg_15e2;
		SFC::Function main_15e3;
	};

	std::list< Match> _matches;
};

class DataInput_15e4
{
public:
	void operator()( const Packets_t& states_15e5, const Packets_t& mains_15e8, Packets_t& args_15e7, Packets_t& mains_15ea);

protected:
	bool isInputUnique( const Udm::Object& state_15f1, const Udm::Object& main_15fa);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::DT& DT, CyberComposition::Simulink::SFData& Data, SFC::Function& Main, CyberComposition::Simulink::SFState& State, CyberComposition::Simulink::SF_TypeBase& TypeBase, CyberComposition::Simulink::TypeBaseRef& TypeBaseRef);
	void processInputPackets( const Packets_t& states_15e5, const Packets_t& mains_15e8);
	bool patternMatcher( const Udm::Object& state_15ef, const Udm::Object& main_15f8);
	void effector();
	void outputAppender( const SFC::Arg& arg_1618, const SFC::Function& main_161a);

private:
	Packets_t* _arg_15eb;
	Packets_t* _main_15ec;
	Packets_t _state_15ed;
	Packets_t _main_15f6;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState state_160b;
		SFC::Function main_160c;
		CyberComposition::Simulink::SF_TypeBase typeBase_160d;
		SFC::DT dT_160e;
		CyberComposition::Simulink::SFData data_160f;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_1610;
	};

	std::list< Match> _matches;
};

class EventInput_1622
{
public:
	void operator()( const Packets_t& states_1623, const Packets_t& functions_1625, Packets_t& states_1627, Packets_t& functions_1628);

protected:
	void callEventInput_1663( const Packets_t& states_162c, const Packets_t& mains_162f);
	void callUpdateArgCount_1666( const Packets_t& args_15c5, const Packets_t& mains_15c7);

private:
	Packets_t* _state_1629;
	Packets_t* _function_162a;
};

class EventInput_162b
{
public:
	void operator()( const Packets_t& states_162c, const Packets_t& mains_162f, Packets_t& args_162e, Packets_t& mains_1631);

protected:
	bool isInputUnique( const Udm::Object& state_1638, const Udm::Object& main_1641);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::DT& DT, CyberComposition::Simulink::SFEvent& Event, SFC::Function& Main, CyberComposition::Simulink::SFState& State, CyberComposition::Simulink::SF_TypeBase& TypeBase, CyberComposition::Simulink::TypeBaseRef& TypeBaseRef);
	void processInputPackets( const Packets_t& states_162c, const Packets_t& mains_162f);
	bool patternMatcher( const Udm::Object& state_1636, const Udm::Object& main_163f);
	void effector();
	void outputAppender( const SFC::Arg& arg_165f, const SFC::Function& main_1661);

private:
	Packets_t* _arg_1632;
	Packets_t* _main_1633;
	Packets_t _state_1634;
	Packets_t _main_163d;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState state_1652;
		SFC::Function main_1653;
		CyberComposition::Simulink::SF_TypeBase typeBase_1654;
		SFC::DT dT_1655;
		CyberComposition::Simulink::SFEvent event_1656;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_1657;
	};

	std::list< Match> _matches;
};

class DataOutput_1669
{
public:
	void operator()( const Packets_t& states_166a, const Packets_t& functions_166c, Packets_t& states_166e, Packets_t& functions_166f);

protected:
	void callDataOutput_16aa( const Packets_t& states_1673, const Packets_t& mains_1676);
	void callUpdateArgCount_16ad( const Packets_t& args_15c5, const Packets_t& mains_15c7);

private:
	Packets_t* _state_1670;
	Packets_t* _function_1671;
};

class DataOutput_1672
{
public:
	void operator()( const Packets_t& states_1673, const Packets_t& mains_1676, Packets_t& args_1675, Packets_t& mains_1678);

protected:
	bool isInputUnique( const Udm::Object& state_167f, const Udm::Object& main_1688);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::DT& DT, CyberComposition::Simulink::SFData& Data, SFC::Function& Main, CyberComposition::Simulink::SFState& State, CyberComposition::Simulink::SF_TypeBase& TypeBase, CyberComposition::Simulink::TypeBaseRef& TypeBaseRef);
	void processInputPackets( const Packets_t& states_1673, const Packets_t& mains_1676);
	bool patternMatcher( const Udm::Object& state_167d, const Udm::Object& main_1686);
	void effector();
	void outputAppender( const SFC::Arg& arg_16a6, const SFC::Function& main_16a8);

private:
	Packets_t* _arg_1679;
	Packets_t* _main_167a;
	Packets_t _state_167b;
	Packets_t _main_1684;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState state_1699;
		SFC::Function main_169a;
		CyberComposition::Simulink::SF_TypeBase typeBase_169b;
		SFC::DT dT_169c;
		CyberComposition::Simulink::SFData data_169d;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_169e;
	};

	std::list< Match> _matches;
};

class EventOutput_16b0
{
public:
	void operator()( const Packets_t& states_16b1, const Packets_t& functions_16b3);

protected:
	void callEventOutput_16ee( const Packets_t& states_16b6, const Packets_t& mains_16b9);
	void callUpdateArgCount_16f1( const Packets_t& args_15c5, const Packets_t& mains_15c7);
};

class EventOutput_16b5
{
public:
	void operator()( const Packets_t& states_16b6, const Packets_t& mains_16b9, Packets_t& args_16b8, Packets_t& mains_16bb);

protected:
	bool isInputUnique( const Udm::Object& state_16c2, const Udm::Object& main_16cb);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::DT& DT, CyberComposition::Simulink::SFEvent& Event, SFC::Function& Main, CyberComposition::Simulink::SFState& State, CyberComposition::Simulink::TypeBaseRef& TypeBaseRef, CyberComposition::Simulink::TypeStruct& TypeStruct);
	void processInputPackets( const Packets_t& states_16b6, const Packets_t& mains_16b9);
	bool patternMatcher( const Udm::Object& state_16c0, const Udm::Object& main_16c9);
	void effector();
	void outputAppender( const SFC::Arg& arg_16ea, const SFC::Function& main_16ec);

private:
	Packets_t* _arg_16bc;
	Packets_t* _main_16bd;
	Packets_t _state_16be;
	Packets_t _main_16c7;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState state_16dd;
		SFC::Function main_16de;
		CyberComposition::Simulink::TypeStruct typeStruct_16df;
		SFC::DT dT_16e0;
		CyberComposition::Simulink::SFEvent event_16e1;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_16e2;
	};

	std::list< Match> _matches;
};

class CreateSFChartFunction_1700
{
public:
	void operator()( const Packets_t& subsystems_1702, Packets_t& states_1701, Packets_t& mains_1704);

protected:
	bool isInputUnique( const Udm::Object& subsystem_170b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1702);
	bool patternMatcher( const Udm::Object& subsystem_1709);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& state_1721, const SFC::Function& main_1723);

private:
	Packets_t* _state_1705;
	Packets_t* _main_1706;
	Packets_t _subsystem_1707;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_171b;
		CyberComposition::Simulink::SFState state_171c;
		SFC::Class class_171d;
		SFC::Struct struct_171e;
	};

	std::list< Match> _matches;
};

class CreateClasses_1738
{
public:
	void operator()( const Packets_t& systems_1739, const Packets_t& programs_173b, Packets_t& systems_173d);

protected:
	void callCopyClass_18d8( const Packets_t& systems_1876, const Packets_t& programs_1878);
	void callGetSubSubsystems_18db( const Packets_t& subsystems_1740, const Packets_t& programs_1743);
	void callSubsystemFilter_18de( const Packets_t& systems_17ab, const Packets_t& programs_17ad);
	void callCreateChartClass_18e1( const Packets_t& subsystems_1769, const Packets_t& programs_176b);
	void callCreateClass_18e4( const Packets_t& subsystems_1788, const Packets_t& programs_178b);
	void callCreateSFChartClass_18e7( const Packets_t& subsystems_18ae, const Packets_t& sFStates_18b0, const Packets_t& programs_18b2);

private:
	Packets_t* _system_173e;
};

class GetSubSubsystems_173f
{
public:
	void operator()( const Packets_t& subsystems_1740, const Packets_t& programs_1743, Packets_t& subSubsystems_1742, Packets_t& programs_1745);

protected:
	bool isInputUnique( const Udm::Object& subsystem_174c, const Udm::Object& program_1755);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1740, const Packets_t& programs_1743);
	bool patternMatcher( const Udm::Object& subsystem_174a, const Udm::Object& program_1753);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subSubsystem_1764, const SFC::Program& program_1766);

private:
	Packets_t* _subSubsystem_1746;
	Packets_t* _program_1747;
	Packets_t _subsystem_1748;
	Packets_t _program_1751;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1761;
		SFC::Program program_1762;
		CyberComposition::Simulink::Subsystem subSubsystem_1763;
	};

	std::list< Match> _matches;
};

class CreateChartClass_1768
{
public:
	void operator()( const Packets_t& subsystems_1769, const Packets_t& programs_176b);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1771, const Udm::Object& program_177a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1769, const Packets_t& programs_176b);
	bool patternMatcher( const Udm::Object& subsystem_176f, const Udm::Object& program_1778);
	void effector();

private:
	Packets_t _subsystem_176d;
	Packets_t _program_1776;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1783;
		SFC::Program program_1784;
	};

	std::list< Match> _matches;
};

class CreateClass_1787
{
public:
	void operator()( const Packets_t& subsystems_1788, const Packets_t& programs_178b, Packets_t& subsystems_178a, Packets_t& programs_178d);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1794, const Udm::Object& program_179d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1788, const Packets_t& programs_178b);
	bool patternMatcher( const Udm::Object& subsystem_1792, const Udm::Object& program_179b);
	void effector();
	void forwardInputs();

private:
	Packets_t* _subsystem_178e;
	Packets_t* _program_178f;
	Packets_t _subsystem_1790;
	Packets_t _program_1799;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_17a6;
		SFC::Program program_17a7;
	};

	std::list< Match> _matches;
};

class SubsystemFilter_17aa
{
public:
	void operator()( const Packets_t& systems_17ab, const Packets_t& programs_17ad, Packets_t& systems_17af, Packets_t& programs_17b0, Packets_t& systems_17b1, Packets_t& programs_17b2, Packets_t& systems_17b3, Packets_t& sFStates_17b4, Packets_t& programs_17b5, Packets_t& systems_17b6, Packets_t& programs_17b7);

protected:
	void executeOne( const Packets_t& systems_17ab, const Packets_t& programs_17ad);
	bool isInputUnique( const Udm::Object& system_17c3, const Udm::Object& program_17ca);

private:
	Packets_t* _system_17b8;
	Packets_t* _program_17b9;
	Packets_t* _system_17ba;
	Packets_t* _program_17bb;
	Packets_t* _system_17bc;
	Packets_t* _sFState_17bd;
	Packets_t* _program_17be;
	Packets_t* _system_17bf;
	Packets_t* _program_17c0;
	Packets_t _system_17c1;
	Packets_t _program_17c8;
};

class HasClass_17cf
{
public:
	bool operator()( const Packets_t& subsystems_17d0, const Packets_t& programs_17d3, Packets_t& subsystems_17d2, Packets_t& programs_17d5);

protected:
	bool isInputUnique( const Udm::Object& subsystem_17dc, const Udm::Object& program_17e5);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_17d0, const Packets_t& programs_17d3);
	bool patternMatcher( const Udm::Object& subsystem_17da, const Udm::Object& program_17e3);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_17f2, const SFC::Program& program_17f4);

private:
	Packets_t* _subsystem_17d6;
	Packets_t* _program_17d7;
	Packets_t _subsystem_17d8;
	Packets_t _program_17e1;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_17ef;
		SFC::Program program_17f0;
		SFC::Class class_17f1;
	};

	std::list< Match> _matches;
};

class IsStateChart_17f6
{
public:
	bool operator()( const Packets_t& subsystems_17f7, const Packets_t& programs_17fa, Packets_t& subsystems_17f9, Packets_t& programs_17fc);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1803, const Udm::Object& program_180c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_17f7, const Packets_t& programs_17fa);
	bool patternMatcher( const Udm::Object& subsystem_1801, const Udm::Object& program_180a);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_181f, const SFC::Program& program_1821);

private:
	Packets_t* _subsystem_17fd;
	Packets_t* _program_17fe;
	Packets_t _subsystem_17ff;
	Packets_t _program_1808;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_181b;
		SFC::Program program_181c;
		CyberComposition::Simulink::Primitive primitive_181d;
		CyberComposition::Simulink::State state_181e;
	};

	std::list< Match> _matches;
};

class IsSFStateChart_1823
{
public:
	bool operator()( const Packets_t& subsystems_1824, const Packets_t& programs_1828, Packets_t& subsystems_1826, Packets_t& sFStates_1827, Packets_t& programs_182a);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1832, const Udm::Object& program_183b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1824, const Packets_t& programs_1828);
	bool patternMatcher( const Udm::Object& subsystem_1830, const Udm::Object& program_1839);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_184a, const CyberComposition::Simulink::SFState& sFState_184c, const SFC::Program& program_184e);

private:
	Packets_t* _subsystem_182b;
	Packets_t* _sFState_182c;
	Packets_t* _program_182d;
	Packets_t _subsystem_182e;
	Packets_t _program_1837;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1847;
		SFC::Program program_1848;
		CyberComposition::Simulink::SFState sFState_1849;
	};

	std::list< Match> _matches;
};

class Otherwise_1850
{
public:
	bool operator()( const Packets_t& subsystems_1851, const Packets_t& programs_1854, Packets_t& subsystems_1853, Packets_t& programs_1856);

protected:
	bool isInputUnique( const Udm::Object& subsystem_185d, const Udm::Object& program_1866);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1851, const Packets_t& programs_1854);
	bool patternMatcher( const Udm::Object& subsystem_185b, const Udm::Object& program_1864);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_1871, const SFC::Program& program_1873);

private:
	Packets_t* _subsystem_1857;
	Packets_t* _program_1858;
	Packets_t _subsystem_1859;
	Packets_t _program_1862;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_186f;
		SFC::Program program_1870;
	};

	std::list< Match> _matches;
};

class CopyClass_1875
{
public:
	void operator()( const Packets_t& systems_1876, const Packets_t& programs_1878, Packets_t& systems_187a, Packets_t& programs_187b);

protected:
	void callCopyClass_18aa( const Packets_t& subsystems_187f, const Packets_t& programs_1881);

private:
	Packets_t* _system_187c;
	Packets_t* _program_187d;
};

class CopyClass_187e
{
public:
	void operator()( const Packets_t& subsystems_187f, const Packets_t& programs_1881);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1887, const Udm::Object& program_1890);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Class& Class, CyberComposition::Simulink::Subsystem& InstanceSubsystem, SFC::Program& Program, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& subsystems_187f, const Packets_t& programs_1881);
	bool patternMatcher( const Udm::Object& subsystem_1885, const Udm::Object& program_188e);
	void effector();

private:
	Packets_t _subsystem_1883;
	Packets_t _program_188c;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_18a0;
		SFC::Program program_18a1;
		SFC::Class class_18a2;
		CyberComposition::Simulink::Subsystem instanceSubsystem_18a3;
	};

	std::list< Match> _matches;
};

class CreateSFChartClass_18ad
{
public:
	void operator()( const Packets_t& subsystems_18ae, const Packets_t& sFStates_18b0, const Packets_t& programs_18b2);

protected:
	bool isInputUnique( const Udm::Object& subsystem_18b8, const Udm::Object& sFState_18c1, const Udm::Object& program_18ca);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_18ae, const Packets_t& sFStates_18b0, const Packets_t& programs_18b2);
	bool patternMatcher( const Udm::Object& subsystem_18b6, const Udm::Object& sFState_18bf, const Udm::Object& program_18c8);
	void effector();

private:
	Packets_t _subsystem_18b4;
	Packets_t _sFState_18bd;
	Packets_t _program_18c6;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_18d3;
		CyberComposition::Simulink::SFState sFState_18d4;
		SFC::Program program_18d5;
	};

	std::list< Match> _matches;
};

class CreateFunctionLocalVars_18eb
{
public:
	void operator()( const Packets_t& systems_18ec, Packets_t& systems_18ee);

protected:
	void callSubsystemFilter_1c36( const Packets_t& systems_10d2);
	void callGetFunctions_1c38( const Packets_t& systems_1c13);
	void callCreateOutPortLVs_1c3a( const Packets_t& systems_1a66, const Packets_t& systemFunctions_1a68);
	void callCreateInPortLVs_1c3d( const Packets_t& systems_18f1, const Packets_t& systemFunctions_18f3);
	void callGetSubSubsystems_1c40( const Packets_t& systems_10b2);

private:
	Packets_t* _system_18ef;
};

class CreateInPortLVs_18f0
{
public:
	void operator()( const Packets_t& systems_18f1, const Packets_t& systemFunctions_18f3, Packets_t& systems_18f5);

protected:
	void callGetChildInPorts_1a55( const Packets_t& subsystems_1a29, const Packets_t& mains_1a2c);
	void callInPortFilter_1a58( const Packets_t& inPorts_1979, const Packets_t& systemFunctions_197b);
	void callGetLocalVars_1a5b( const Packets_t& inPorts_1915);
	void callGetInArgs_1a5d( const Packets_t& inPorts_192f);
	void callCreateLocalVars_1a5f( const Packets_t& inPorts_194c, const Packets_t& mains_194e);
	void callZeroInitLocalVars_1a62( const Packets_t& localVars_18f8, const Packets_t& mains_18fa);

private:
	Packets_t* _system_18f6;
};

class ZeroInitLocalVars_18f7
{
public:
	void operator()( const Packets_t& localVars_18f8, const Packets_t& mains_18fa);

protected:
	bool isInputUnique( const Udm::Object& localVar_1900, const Udm::Object& main_1909);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& localVars_18f8, const Packets_t& mains_18fa);
	bool patternMatcher( const Udm::Object& localVar_18fe, const Udm::Object& main_1907);
	void effector();

private:
	Packets_t _localVar_18fc;
	Packets_t _main_1905;
	class Match
	{
	public:
		SFC::LocalVar localVar_1912;
		SFC::Function main_1913;
	};

	std::list< Match> _matches;
};

class GetLocalVars_1914
{
public:
	void operator()( const Packets_t& inPorts_1915);

protected:
	bool isInputUnique( const Udm::Object& inPort_191b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& inPorts_1915);
	bool patternMatcher( const Udm::Object& inPort_1919);
	void effector();

private:
	Packets_t _inPort_1917;
	class Match
	{
	public:
		CyberComposition::Simulink::InPort inPort_192a;
		SFC::ArgDeclBase argDeclBase_192b;
		CyberComposition::Simulink::OutPort outPort_192c;
		CyberComposition::Simulink::Line line_192d;
	};

	std::list< Match> _matches;
};

class GetInArgs_192e
{
public:
	void operator()( const Packets_t& inPorts_192f);

protected:
	bool isInputUnique( const Udm::Object& inPort_1935);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& inPorts_192f);
	bool patternMatcher( const Udm::Object& inPort_1933);
	void effector();

private:
	Packets_t _inPort_1931;
	class Match
	{
	public:
		CyberComposition::Simulink::InPort inPort_1946;
		SFC::Arg arg_1947;
		CyberComposition::Simulink::InPort subsystemInPort_1948;
		CyberComposition::Simulink::Line line_1949;
	};

	std::list< Match> _matches;
};

class CreateLocalVars_194a
{
public:
	void operator()( const Packets_t& inPorts_194c, const Packets_t& mains_194e, Packets_t& localVars_194b, Packets_t& mains_1950);

protected:
	bool isInputUnique( const Udm::Object& inPort_1957, const Udm::Object& main_1960);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& inPorts_194c, const Packets_t& mains_194e);
	bool patternMatcher( const Udm::Object& inPort_1955, const Udm::Object& main_195e);
	void effector();
	void outputAppender( const SFC::LocalVar& localVar_1974, const SFC::Function& main_1976);

private:
	Packets_t* _localVar_1951;
	Packets_t* _main_1952;
	Packets_t _inPort_1953;
	Packets_t _main_195c;
	class Match
	{
	public:
		CyberComposition::Simulink::InPort inPort_196e;
		SFC::Function main_196f;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_1970;
		SFC::DT dT_1971;
		CyberComposition::Simulink::SF_TypeBase typeBase_1972;
	};

	std::list< Match> _matches;
};

class InPortFilter_1978
{
public:
	void operator()( const Packets_t& inPorts_1979, const Packets_t& systemFunctions_197b, Packets_t& inPorts_197d, Packets_t& systemFunctions_197e, Packets_t& inPorts_197f, Packets_t& systemFunctions_1980, Packets_t& inPorts_1981, Packets_t& systemFunctions_1982);

protected:
	void callInPortTest_1a25( const Packets_t& inPorts_198a, const Packets_t& systemFunctions_198c);

private:
	Packets_t* _inPort_1983;
	Packets_t* _systemFunction_1984;
	Packets_t* _inPort_1985;
	Packets_t* _systemFunction_1986;
	Packets_t* _inPort_1987;
	Packets_t* _systemFunction_1988;
};

class InPortTest_1989
{
public:
	void operator()( const Packets_t& inPorts_198a, const Packets_t& systemFunctions_198c, Packets_t& inPorts_198e, Packets_t& systemFunctions_198f, Packets_t& inPorts_1990, Packets_t& systemFunctions_1991, Packets_t& inPorts_1992, Packets_t& systemFunctions_1993);

protected:
	void executeOne( const Packets_t& inPorts_198a, const Packets_t& systemFunctions_198c);
	bool isInputUnique( const Udm::Object& inPort_199c, const Udm::Object& systemFunction_19a3);

private:
	Packets_t* _inPort_1994;
	Packets_t* _systemFunction_1995;
	Packets_t* _inPort_1996;
	Packets_t* _systemFunction_1997;
	Packets_t* _inPort_1998;
	Packets_t* _systemFunction_1999;
	Packets_t _inPort_199a;
	Packets_t _systemFunction_19a1;
};

class InPort2InPort_19a8
{
public:
	bool operator()( const Packets_t& inPorts_19a9, const Packets_t& mains_19ac, Packets_t& inPorts_19ab, Packets_t& mains_19ae);

protected:
	bool isInputUnique( const Udm::Object& inPort_19b5, const Udm::Object& main_19be);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& inPorts_19a9, const Packets_t& mains_19ac);
	bool patternMatcher( const Udm::Object& inPort_19b3, const Udm::Object& main_19bc);
	void outputAppender( const CyberComposition::Simulink::InPort& inPort_19d0, const SFC::Function& main_19d2);

private:
	Packets_t* _inPort_19af;
	Packets_t* _main_19b0;
	Packets_t _inPort_19b1;
	Packets_t _main_19ba;
	class Match
	{
	public:
		CyberComposition::Simulink::InPort inPort_19cc;
		SFC::Function main_19cd;
		CyberComposition::Simulink::InPort subsystemInPort_19ce;
		CyberComposition::Simulink::Line line_19cf;
	};

	std::list< Match> _matches;
};

class OutPort2InPort_19d4
{
public:
	bool operator()( const Packets_t& inPorts_19d5, const Packets_t& mains_19d8, Packets_t& inPorts_19d7, Packets_t& mains_19da);

protected:
	bool isInputUnique( const Udm::Object& inPort_19e1, const Udm::Object& main_19ea);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& inPorts_19d5, const Packets_t& mains_19d8);
	bool patternMatcher( const Udm::Object& inPort_19df, const Udm::Object& main_19e8);
	void outputAppender( const CyberComposition::Simulink::InPort& inPort_19fc, const SFC::Function& main_19fe);

private:
	Packets_t* _inPort_19db;
	Packets_t* _main_19dc;
	Packets_t _inPort_19dd;
	Packets_t _main_19e6;
	class Match
	{
	public:
		CyberComposition::Simulink::InPort inPort_19f8;
		SFC::Function main_19f9;
		CyberComposition::Simulink::OutPort outPort_19fa;
		CyberComposition::Simulink::Line line_19fb;
	};

	std::list< Match> _matches;
};

class Otherwise_1a00
{
public:
	bool operator()( const Packets_t& inPorts_1a01, const Packets_t& mains_1a04, Packets_t& inPorts_1a03, Packets_t& mains_1a06);

protected:
	bool isInputUnique( const Udm::Object& inPort_1a0d, const Udm::Object& main_1a16);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& inPorts_1a01, const Packets_t& mains_1a04);
	bool patternMatcher( const Udm::Object& inPort_1a0b, const Udm::Object& main_1a14);
	void outputAppender( const CyberComposition::Simulink::InPort& inPort_1a21, const SFC::Function& main_1a23);

private:
	Packets_t* _inPort_1a07;
	Packets_t* _main_1a08;
	Packets_t _inPort_1a09;
	Packets_t _main_1a12;
	class Match
	{
	public:
		CyberComposition::Simulink::InPort inPort_1a1f;
		SFC::Function main_1a20;
	};

	std::list< Match> _matches;
};

class GetChildInPorts_1a28
{
public:
	void operator()( const Packets_t& subsystems_1a29, const Packets_t& mains_1a2c, Packets_t& inPorts_1a2b, Packets_t& mains_1a2e);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1a35, const Udm::Object& main_1a3e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1a29, const Packets_t& mains_1a2c);
	bool patternMatcher( const Udm::Object& subsystem_1a33, const Udm::Object& main_1a3c);
	void effector();
	void outputAppender( const CyberComposition::Simulink::InPort& inPort_1a51, const SFC::Function& main_1a53);

private:
	Packets_t* _inPort_1a2f;
	Packets_t* _main_1a30;
	Packets_t _subsystem_1a31;
	Packets_t _main_1a3a;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1a4d;
		SFC::Function main_1a4e;
		CyberComposition::Simulink::Block block_1a4f;
		CyberComposition::Simulink::InPort inPort_1a50;
	};

	std::list< Match> _matches;
};

class CreateOutPortLVs_1a65
{
public:
	void operator()( const Packets_t& systems_1a66, const Packets_t& systemFunctions_1a68, Packets_t& systems_1a6a, Packets_t& systemFunctions_1a6b);

protected:
	void callGetChildOutPorts_1c04( const Packets_t& subsystems_1b5c, const Packets_t& mains_1b5f);
	void callOutPortFilter_1c07( const Packets_t& outPorts_1b89, const Packets_t& systemFunctions_1b8b);
	void callGetOutArgs_1c0a( const Packets_t& outPorts_1a70);
	void callCreateLocalVars_1c0c( const Packets_t& outPorts_1b36, const Packets_t& mains_1b38);
	void callBindToArg_1c0f( const Packets_t& systemOutPorts_1a8f, const Packets_t& outPorts_1a91);

private:
	Packets_t* _system_1a6c;
	Packets_t* _systemFunction_1a6d;
};

class GetOutArgs_1a6e
{
public:
	void operator()( const Packets_t& outPorts_1a70, Packets_t& subsystemOutPorts_1a6f, Packets_t& outPorts_1a72);

protected:
	bool isInputUnique( const Udm::Object& outPort_1a79);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& outPorts_1a70);
	bool patternMatcher( const Udm::Object& outPort_1a77);
	void effector();
	void outputAppender( const CyberComposition::Simulink::OutPort& subsystemOutPort_1a8a, const CyberComposition::Simulink::OutPort& outPort_1a8c);
	void sortOutputs();

private:
	Packets_t* _subsystemOutPort_1a73;
	Packets_t* _outPort_1a74;
	Packets_t _outPort_1a75;
	class Match
	{
	public:
		CyberComposition::Simulink::OutPort outPort_1a87;
		CyberComposition::Simulink::OutPort subsystemOutPort_1a88;
		CyberComposition::Simulink::Line line_1a89;
	};

	std::list< Match> _matches;
};

class BindToArg_1a8e
{
public:
	void operator()( const Packets_t& systemOutPorts_1a8f, const Packets_t& outPorts_1a91);

protected:
	void executeOne( const Packets_t& systemOutPorts_1a8f, const Packets_t& outPorts_1a91);
	bool isInputUnique( const Udm::Object& systemOutPort_1a95, const Udm::Object& outPort_1a9c);
	void callOutputArgTest_1b2f( const Packets_t& systemOutPorts_1ac9, const Packets_t& outPorts_1acb);
	void callBindToArg_1b32( const Packets_t& subsystemOutPorts_1aa2, const Packets_t& outPorts_1aa4);

private:
	Packets_t _systemOutPort_1a93;
	Packets_t _outPort_1a9a;
};

class BindToArg_1aa1
{
public:
	void operator()( const Packets_t& subsystemOutPorts_1aa2, const Packets_t& outPorts_1aa4);

protected:
	bool isInputUnique( const Udm::Object& subsystemOutPort_1aaa, const Udm::Object& outPort_1ab3);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystemOutPorts_1aa2, const Packets_t& outPorts_1aa4);
	bool patternMatcher( const Udm::Object& subsystemOutPort_1aa8, const Udm::Object& outPort_1ab1);
	void effector();

private:
	Packets_t _subsystemOutPort_1aa6;
	Packets_t _outPort_1aaf;
	class Match
	{
	public:
		CyberComposition::Simulink::OutPort subsystemOutPort_1ac4;
		CyberComposition::Simulink::OutPort outPort_1ac5;
		SFC::Arg arg_1ac6;
		CyberComposition::Simulink::Line line_1ac7;
	};

	std::list< Match> _matches;
};

class OutputArgTest_1ac8
{
public:
	void operator()( const Packets_t& systemOutPorts_1ac9, const Packets_t& outPorts_1acb, Packets_t& systemOutPorts_1acd, Packets_t& outPorts_1ace, Packets_t& systemOutPorts_1acf, Packets_t& outPorts_1ad0);

protected:
	void executeOne( const Packets_t& systemOutPorts_1ac9, const Packets_t& outPorts_1acb);
	bool isInputUnique( const Udm::Object& systemOutPort_1ad7, const Udm::Object& outPort_1ade);

private:
	Packets_t* _systemOutPort_1ad1;
	Packets_t* _outPort_1ad2;
	Packets_t* _systemOutPort_1ad3;
	Packets_t* _outPort_1ad4;
	Packets_t _systemOutPort_1ad5;
	Packets_t _outPort_1adc;
};

class OutPortArg_1ae3
{
public:
	bool operator()( const Packets_t& subsystemOutPorts_1ae4, const Packets_t& outPorts_1ae7, Packets_t& subsystemOutPorts_1ae6, Packets_t& outPorts_1ae9);

protected:
	bool isInputUnique( const Udm::Object& subsystemOutPort_1af0, const Udm::Object& outPort_1af9);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystemOutPorts_1ae4, const Packets_t& outPorts_1ae7);
	bool patternMatcher( const Udm::Object& subsystemOutPort_1aee, const Udm::Object& outPort_1af7);
	void outputAppender( const CyberComposition::Simulink::OutPort& subsystemOutPort_1b06, const CyberComposition::Simulink::OutPort& outPort_1b08);

private:
	Packets_t* _subsystemOutPort_1aea;
	Packets_t* _outPort_1aeb;
	Packets_t _subsystemOutPort_1aec;
	Packets_t _outPort_1af5;
	class Match
	{
	public:
		CyberComposition::Simulink::OutPort subsystemOutPort_1b03;
		CyberComposition::Simulink::OutPort outPort_1b04;
		SFC::ArgDeclBase argDeclBase_1b05;
	};

	std::list< Match> _matches;
};

class Otherwise_1b0a
{
public:
	bool operator()( const Packets_t& subsystemOutPorts_1b0b, const Packets_t& outPorts_1b0e, Packets_t& subsystemOutPorts_1b0d, Packets_t& outPorts_1b10);

protected:
	bool isInputUnique( const Udm::Object& subsystemOutPort_1b17, const Udm::Object& outPort_1b20);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystemOutPorts_1b0b, const Packets_t& outPorts_1b0e);
	bool patternMatcher( const Udm::Object& subsystemOutPort_1b15, const Udm::Object& outPort_1b1e);
	void outputAppender( const CyberComposition::Simulink::OutPort& subsystemOutPort_1b2b, const CyberComposition::Simulink::OutPort& outPort_1b2d);

private:
	Packets_t* _subsystemOutPort_1b11;
	Packets_t* _outPort_1b12;
	Packets_t _subsystemOutPort_1b13;
	Packets_t _outPort_1b1c;
	class Match
	{
	public:
		CyberComposition::Simulink::OutPort subsystemOutPort_1b29;
		CyberComposition::Simulink::OutPort outPort_1b2a;
	};

	std::list< Match> _matches;
};

class CreateLocalVars_1b35
{
public:
	void operator()( const Packets_t& outPorts_1b36, const Packets_t& mains_1b38);

protected:
	bool isInputUnique( const Udm::Object& outPort_1b3e, const Udm::Object& main_1b47);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& outPorts_1b36, const Packets_t& mains_1b38);
	bool patternMatcher( const Udm::Object& outPort_1b3c, const Udm::Object& main_1b45);
	void effector();

private:
	Packets_t _outPort_1b3a;
	Packets_t _main_1b43;
	class Match
	{
	public:
		CyberComposition::Simulink::OutPort outPort_1b55;
		SFC::Function main_1b56;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_1b57;
		CyberComposition::Simulink::SF_TypeBase typeBase_1b58;
		SFC::DT dT_1b59;
	};

	std::list< Match> _matches;
};

class GetChildOutPorts_1b5b
{
public:
	void operator()( const Packets_t& subsystems_1b5c, const Packets_t& mains_1b5f, Packets_t& outPorts_1b5e, Packets_t& mains_1b61);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1b68, const Udm::Object& main_1b71);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1b5c, const Packets_t& mains_1b5f);
	bool patternMatcher( const Udm::Object& subsystem_1b66, const Udm::Object& main_1b6f);
	void effector();
	void outputAppender( const CyberComposition::Simulink::OutPort& outPort_1b84, const SFC::Function& main_1b86);

private:
	Packets_t* _outPort_1b62;
	Packets_t* _main_1b63;
	Packets_t _subsystem_1b64;
	Packets_t _main_1b6d;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1b80;
		SFC::Function main_1b81;
		CyberComposition::Simulink::Block block_1b82;
		CyberComposition::Simulink::OutPort outPort_1b83;
	};

	std::list< Match> _matches;
};

class OutPortFilter_1b88
{
public:
	void operator()( const Packets_t& outPorts_1b89, const Packets_t& systemFunctions_1b8b, Packets_t& outPorts_1b8d, Packets_t& systemFunctions_1b8e, Packets_t& outPorts_1b8f, Packets_t& systemFunctions_1b90);

protected:
	void callOutPortTest_1c01( const Packets_t& outPorts_1b96, const Packets_t& systemFunctions_1b98);

private:
	Packets_t* _outPort_1b91;
	Packets_t* _systemFunction_1b92;
	Packets_t* _outPort_1b93;
	Packets_t* _systemFunction_1b94;
};

class OutPortTest_1b95
{
public:
	void operator()( const Packets_t& outPorts_1b96, const Packets_t& systemFunctions_1b98, Packets_t& outPorts_1b9a, Packets_t& systemFunctions_1b9b, Packets_t& outPorts_1b9c, Packets_t& systemFunctions_1b9d);

protected:
	void executeOne( const Packets_t& outPorts_1b96, const Packets_t& systemFunctions_1b98);
	bool isInputUnique( const Udm::Object& outPort_1ba4, const Udm::Object& systemFunction_1bab);

private:
	Packets_t* _outPort_1b9e;
	Packets_t* _systemFunction_1b9f;
	Packets_t* _outPort_1ba0;
	Packets_t* _systemFunction_1ba1;
	Packets_t _outPort_1ba2;
	Packets_t _systemFunction_1ba9;
};

class OutPort2OutPort_1bb0
{
public:
	bool operator()( const Packets_t& outPorts_1bb1, const Packets_t& mains_1bb4, Packets_t& outPorts_1bb3, Packets_t& mains_1bb6);

protected:
	bool isInputUnique( const Udm::Object& outPort_1bbd, const Udm::Object& main_1bc6);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& outPorts_1bb1, const Packets_t& mains_1bb4);
	bool patternMatcher( const Udm::Object& outPort_1bbb, const Udm::Object& main_1bc4);
	void outputAppender( const CyberComposition::Simulink::OutPort& outPort_1bd8, const SFC::Function& main_1bda);

private:
	Packets_t* _outPort_1bb7;
	Packets_t* _main_1bb8;
	Packets_t _outPort_1bb9;
	Packets_t _main_1bc2;
	class Match
	{
	public:
		CyberComposition::Simulink::OutPort outPort_1bd4;
		SFC::Function main_1bd5;
		CyberComposition::Simulink::OutPort subsystemOutPort_1bd6;
		CyberComposition::Simulink::Line line_1bd7;
	};

	std::list< Match> _matches;
};

class Otherwise_1bdc
{
public:
	bool operator()( const Packets_t& outPorts_1bdd, const Packets_t& mains_1be0, Packets_t& outPorts_1bdf, Packets_t& mains_1be2);

protected:
	bool isInputUnique( const Udm::Object& outPort_1be9, const Udm::Object& main_1bf2);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& outPorts_1bdd, const Packets_t& mains_1be0);
	bool patternMatcher( const Udm::Object& outPort_1be7, const Udm::Object& main_1bf0);
	void outputAppender( const CyberComposition::Simulink::OutPort& outPort_1bfd, const SFC::Function& main_1bff);

private:
	Packets_t* _outPort_1be3;
	Packets_t* _main_1be4;
	Packets_t _outPort_1be5;
	Packets_t _main_1bee;
	class Match
	{
	public:
		CyberComposition::Simulink::OutPort outPort_1bfb;
		SFC::Function main_1bfc;
	};

	std::list< Match> _matches;
};

class GetFunctions_1c12
{
public:
	void operator()( const Packets_t& systems_1c13, Packets_t& systems_1c15, Packets_t& systemFunctions_1c16);

protected:
	void callGetFunctions_1c34( const Packets_t& subsystems_1c1a);

private:
	Packets_t* _system_1c17;
	Packets_t* _systemFunction_1c18;
};

class GetFunctions_1c19
{
public:
	void operator()( const Packets_t& subsystems_1c1a, Packets_t& subsystems_1c1c, Packets_t& mains_1c1d);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1c24);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1c1a);
	bool patternMatcher( const Udm::Object& subsystem_1c22);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_1c30, const SFC::Function& main_1c32);

private:
	Packets_t* _subsystem_1c1e;
	Packets_t* _main_1c1f;
	Packets_t _subsystem_1c20;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1c2e;
		SFC::Function main_1c2f;
	};

	std::list< Match> _matches;
};

class CreateProgramsTopLevel_1c42
{
public:
	void operator()( const Packets_t& systems_1c43, const Packets_t& projects_1c45, Packets_t& systems_1c47, Packets_t& programs_1c48);

protected:
	void executeOne( const Packets_t& systems_1c43, const Packets_t& projects_1c45);
	bool isInputUnique( const Udm::Object& system_1c4d, const Udm::Object& project_1c54);
	void callHasClassFilter_1ce8( const Packets_t& systems_1c5a, const Packets_t& projects_1c5c);
	void callCreateProgramClass_1ceb( const Packets_t& subsystems_1cc1, const Packets_t& projects_1cc5);

private:
	Packets_t* _system_1c49;
	Packets_t* _program_1c4a;
	Packets_t _system_1c4b;
	Packets_t _project_1c52;
};

class HasClassFilter_1c59
{
public:
	void operator()( const Packets_t& systems_1c5a, const Packets_t& projects_1c5c, Packets_t& systems_1c5e, Packets_t& projects_1c5f, Packets_t& systems_1c60, Packets_t& projects_1c61);

protected:
	void executeOne( const Packets_t& systems_1c5a, const Packets_t& projects_1c5c);
	bool isInputUnique( const Udm::Object& system_1c68, const Udm::Object& project_1c6f);

private:
	Packets_t* _system_1c62;
	Packets_t* _project_1c63;
	Packets_t* _system_1c64;
	Packets_t* _project_1c65;
	Packets_t _system_1c66;
	Packets_t _project_1c6d;
};

class SubsystemHasClass_1c74
{
public:
	bool operator()( const Packets_t& subsystems_1c75, const Packets_t& projects_1c78, Packets_t& subsystems_1c77, Packets_t& projects_1c7a);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1c81, const Udm::Object& project_1c8a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1c75, const Packets_t& projects_1c78);
	bool patternMatcher( const Udm::Object& subsystem_1c7f, const Udm::Object& project_1c88);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_1c97, const SFC::Project& project_1c99);

private:
	Packets_t* _subsystem_1c7b;
	Packets_t* _project_1c7c;
	Packets_t _subsystem_1c7d;
	Packets_t _project_1c86;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1c94;
		SFC::Project project_1c95;
		SFC::Class class_1c96;
	};

	std::list< Match> _matches;
};

class NoClass_1c9b
{
public:
	bool operator()( const Packets_t& subsystems_1c9c, const Packets_t& projects_1c9f, Packets_t& subsystems_1c9e, Packets_t& projects_1ca1);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1ca8, const Udm::Object& project_1cb1);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1c9c, const Packets_t& projects_1c9f);
	bool patternMatcher( const Udm::Object& subsystem_1ca6, const Udm::Object& project_1caf);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_1cbc, const SFC::Project& project_1cbe);

private:
	Packets_t* _subsystem_1ca2;
	Packets_t* _project_1ca3;
	Packets_t _subsystem_1ca4;
	Packets_t _project_1cad;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1cba;
		SFC::Project project_1cbb;
	};

	std::list< Match> _matches;
};

class CreateProgramClass_1cc0
{
public:
	void operator()( const Packets_t& subsystems_1cc1, const Packets_t& projects_1cc5, Packets_t& subsystems_1cc3, Packets_t& programs_1cc4);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1ccd, const Udm::Object& project_1cd6);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1cc1, const Packets_t& projects_1cc5);
	bool patternMatcher( const Udm::Object& subsystem_1ccb, const Udm::Object& project_1cd4);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_1ce4, const SFC::Program& program_1ce6);

private:
	Packets_t* _subsystem_1cc7;
	Packets_t* _program_1cc8;
	Packets_t _subsystem_1cc9;
	Packets_t _project_1cd2;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1cdf;
		SFC::Project project_1ce0;
	};

	std::list< Match> _matches;
};

class CreateSigFlowsR_1cee
{
public:
	void operator()( const Packets_t& systems_1cef);

protected:
	void callSubsystemFilter_52d9( const Packets_t& systems_10d2);
	void callGetFunctions_52db( const Packets_t& systems_1c13);
	void callCreateTSB_52dd( const Packets_t& systems_1cf2, const Packets_t& systemFunctions_1cf4);
	void callGetSubSubsystems_52e0( const Packets_t& systems_10b2);
};

class CreateTSB_1cf1
{
public:
	void operator()( const Packets_t& systems_1cf2, const Packets_t& systemFunctions_1cf4, Packets_t& systems_1cf6);

protected:
	void callPassThroughs_52c9( const Packets_t& systems_1cf9, const Packets_t& systemFunctions_1cfb);
	void callDoTopologicalSort_52cc( const Packets_t& subsystems_1d65, const Packets_t& mains_1d68);
	void callCreatePreDelayExec_52cf( const Packets_t& systems_1d86, const Packets_t& systemFunctions_1d88);
	void callGetSortedBlocks_52d2( const Packets_t& subsystems_1d38, const Packets_t& mains_1d3c);
	void callCreateBlockExecution_52d5( const Packets_t& systems_1de3, const Packets_t& childBlockss_1de5, const Packets_t& systemFunctions_1de7);

private:
	Packets_t* _system_1cf7;
};

class PassThroughs_1cf8
{
public:
	void operator()( const Packets_t& systems_1cf9, const Packets_t& systemFunctions_1cfb, Packets_t& systems_1cfd, Packets_t& systemFunctions_1cfe);

protected:
	void callPassThroughs_1d34( const Packets_t& subsystems_1d02, const Packets_t& mains_1d04);

private:
	Packets_t* _system_1cff;
	Packets_t* _systemFunction_1d00;
};

class PassThroughs_1d01
{
public:
	void operator()( const Packets_t& subsystems_1d02, const Packets_t& mains_1d04);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1d0a, const Udm::Object& main_1d13);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1d02, const Packets_t& mains_1d04);
	bool patternMatcher( const Udm::Object& subsystem_1d08, const Udm::Object& main_1d11);
	void effector();

private:
	Packets_t _subsystem_1d06;
	Packets_t _main_1d0f;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1d2d;
		SFC::Function main_1d2e;
		CyberComposition::Simulink::Line line_1d2f;
		CyberComposition::Simulink::InPort inPort_1d30;
		CyberComposition::Simulink::OutPort outPort_1d31;
		SFC::Arg outArg_1d32;
		SFC::Arg inArg_1d33;
	};

	std::list< Match> _matches;
};

class GetSortedBlocks_1d37
{
public:
	void operator()( const Packets_t& subsystems_1d38, const Packets_t& mains_1d3c, Packets_t& subsystems_1d3a, Packets_t& childBlockss_1d3b, Packets_t& mains_1d3e);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1d46, const Udm::Object& main_1d4f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1d38, const Packets_t& mains_1d3c);
	bool patternMatcher( const Udm::Object& subsystem_1d44, const Udm::Object& main_1d4d);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_1d5e, const CyberComposition::Simulink::Block& childBlocks_1d60, const SFC::Function& main_1d62);
	void sortOutputs();

private:
	Packets_t* _subsystem_1d3f;
	Packets_t* _childBlocks_1d40;
	Packets_t* _main_1d41;
	Packets_t _subsystem_1d42;
	Packets_t _main_1d4b;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1d5b;
		SFC::Function main_1d5c;
		CyberComposition::Simulink::Block childBlocks_1d5d;
	};

	std::list< Match> _matches;
};

class DoTopologicalSort_1d64
{
public:
	void operator()( const Packets_t& subsystems_1d65, const Packets_t& mains_1d68, Packets_t& subsystems_1d67, Packets_t& mains_1d6a);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1d71, const Udm::Object& main_1d7a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1d65, const Packets_t& mains_1d68);
	bool patternMatcher( const Udm::Object& subsystem_1d6f, const Udm::Object& main_1d78);
	void effector();
	void forwardInputs();

private:
	Packets_t* _subsystem_1d6b;
	Packets_t* _main_1d6c;
	Packets_t _subsystem_1d6d;
	Packets_t _main_1d76;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1d83;
		SFC::Function main_1d84;
	};

	std::list< Match> _matches;
};

class CreatePreDelayExec_1d85
{
public:
	void operator()( const Packets_t& systems_1d86, const Packets_t& systemFunctions_1d88, Packets_t& systems_1d8a, Packets_t& systemFunctions_1d8b);

protected:
	void callGetDelayBlocks_52bf( const Packets_t& subsystems_1d8f, const Packets_t& mains_1d93);
	void callCreateBlockExecution_52c2( const Packets_t& systems_1de3, const Packets_t& childBlockss_1de5, const Packets_t& systemFunctions_1de7);
	void callResetDelayBlockType_52c6( const Packets_t& subsystems_1dbf, const Packets_t& mains_1dc1);

private:
	Packets_t* _system_1d8c;
	Packets_t* _systemFunction_1d8d;
};

class GetDelayBlocks_1d8e
{
public:
	void operator()( const Packets_t& subsystems_1d8f, const Packets_t& mains_1d93, Packets_t& subsystems_1d91, Packets_t& childBlockss_1d92, Packets_t& mains_1d95);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1d9d, const Udm::Object& main_1da6);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::Primitive& ChildBlocks, SFC::Function& Main, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& subsystems_1d8f, const Packets_t& mains_1d93);
	bool patternMatcher( const Udm::Object& subsystem_1d9b, const Udm::Object& main_1da4);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_1db8, const CyberComposition::Simulink::Primitive& childBlocks_1dba, const SFC::Function& main_1dbc);

private:
	Packets_t* _subsystem_1d96;
	Packets_t* _childBlocks_1d97;
	Packets_t* _main_1d98;
	Packets_t _subsystem_1d99;
	Packets_t _main_1da2;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1db2;
		SFC::Function main_1db3;
		CyberComposition::Simulink::Primitive childBlocks_1db4;
	};

	std::list< Match> _matches;
};

class ResetDelayBlockType_1dbe
{
public:
	void operator()( const Packets_t& subsystems_1dbf, const Packets_t& mains_1dc1);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1dc7, const Udm::Object& main_1dd0);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::Primitive& ChildBlocks, SFC::Function& Main, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& subsystems_1dbf, const Packets_t& mains_1dc1);
	bool patternMatcher( const Udm::Object& subsystem_1dc5, const Udm::Object& main_1dce);
	void effector();

private:
	Packets_t _subsystem_1dc3;
	Packets_t _main_1dcc;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1ddc;
		SFC::Function main_1ddd;
		CyberComposition::Simulink::Primitive childBlocks_1dde;
	};

	std::list< Match> _matches;
};

class CreateBlockExecution_1de2
{
public:
	void operator()( const Packets_t& systems_1de3, const Packets_t& childBlockss_1de5, const Packets_t& systemFunctions_1de7, Packets_t& systems_1de9, Packets_t& systemFunctions_1dea);

protected:
	void executeOne( const Packets_t& systems_1de3, const Packets_t& childBlockss_1de5, const Packets_t& systemFunctions_1de7);
	bool isInputUnique( const Udm::Object& system_1def, const Udm::Object& childBlocks_1df6, const Udm::Object& systemFunction_1dfd);
	void callTestChildBlock_52ac( const Packets_t& systems_1e03, const Packets_t& childBlocks_1e05, const Packets_t& systemFunctions_1e07);
	void callPrimitiveChildBlock_52b0( const Packets_t& childPrimitives_2e03, const Packets_t& systemFunctions_2e05);
	void callSubsystemChildBlock_52b3( const Packets_t& systems_1f17, const Packets_t& childSubsystems_1f19, const Packets_t& systemFunctions_1f1b);
	void callChartChildBlock_52b7( const Packets_t& systems_305d, const Packets_t& childCharts_305f, const Packets_t& systemFunctions_3061);
	void callSFChartChildBlock_52bb( const Packets_t& systems_4187, const Packets_t& childCharts_4189, const Packets_t& systemFunctions_418b);

private:
	Packets_t* _system_1deb;
	Packets_t* _systemFunction_1dec;
	Packets_t _system_1ded;
	Packets_t _childBlocks_1df4;
	Packets_t _systemFunction_1dfb;
};

class TestChildBlock_1e02
{
public:
	void operator()( const Packets_t& systems_1e03, const Packets_t& childBlocks_1e05, const Packets_t& systemFunctions_1e07, Packets_t& systems_1e09, Packets_t& childCharts_1e0a, Packets_t& systemFunctions_1e0b, Packets_t& systems_1e0c, Packets_t& childCharts_1e0d, Packets_t& systemFunctions_1e0e, Packets_t& systems_1e0f, Packets_t& childSubsystems_1e10, Packets_t& systemFunctions_1e11, Packets_t& systems_1e12, Packets_t& childBlocks_1e13, Packets_t& systemFunctions_1e14);

protected:
	void executeOne( const Packets_t& systems_1e03, const Packets_t& childBlocks_1e05, const Packets_t& systemFunctions_1e07);
	bool isInputUnique( const Udm::Object& system_1e23, const Udm::Object& childBlock_1e2a, const Udm::Object& systemFunction_1e31);

private:
	Packets_t* _system_1e15;
	Packets_t* _childChart_1e16;
	Packets_t* _systemFunction_1e17;
	Packets_t* _system_1e18;
	Packets_t* _childChart_1e19;
	Packets_t* _systemFunction_1e1a;
	Packets_t* _system_1e1b;
	Packets_t* _childSubsystem_1e1c;
	Packets_t* _systemFunction_1e1d;
	Packets_t* _system_1e1e;
	Packets_t* _childBlock_1e1f;
	Packets_t* _systemFunction_1e20;
	Packets_t _system_1e21;
	Packets_t _childBlock_1e28;
	Packets_t _systemFunction_1e2f;
};

class IsChart_1e36
{
public:
	bool operator()( const Packets_t& subsystems_1e37, const Packets_t& blocks_1e3a, const Packets_t& mains_1e3d, Packets_t& subsystems_1e39, Packets_t& blocks_1e3c, Packets_t& mains_1e3f);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1e47, const Udm::Object& block_1e50, const Udm::Object& main_1e59);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1e37, const Packets_t& blocks_1e3a, const Packets_t& mains_1e3d);
	bool patternMatcher( const Udm::Object& subsystem_1e45, const Udm::Object& block_1e4e, const Udm::Object& main_1e57);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_1e6d, const CyberComposition::Simulink::Subsystem& block_1e6f, const SFC::Function& main_1e71);

private:
	Packets_t* _subsystem_1e40;
	Packets_t* _block_1e41;
	Packets_t* _main_1e42;
	Packets_t _subsystem_1e43;
	Packets_t _block_1e4c;
	Packets_t _main_1e55;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1e68;
		CyberComposition::Simulink::Subsystem block_1e69;
		SFC::Function main_1e6a;
		CyberComposition::Simulink::Primitive primitive_1e6b;
		CyberComposition::Simulink::State state_1e6c;
	};

	std::list< Match> _matches;
};

class IsSFChart_1e73
{
public:
	bool operator()( const Packets_t& subsystems_1e74, const Packets_t& blocks_1e77, const Packets_t& mains_1e7a, Packets_t& subsystems_1e76, Packets_t& blocks_1e79, Packets_t& mains_1e7c);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1e84, const Udm::Object& block_1e8d, const Udm::Object& main_1e96);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1e74, const Packets_t& blocks_1e77, const Packets_t& mains_1e7a);
	bool patternMatcher( const Udm::Object& subsystem_1e82, const Udm::Object& block_1e8b, const Udm::Object& main_1e94);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_1ea6, const CyberComposition::Simulink::Subsystem& block_1ea8, const SFC::Function& main_1eaa);

private:
	Packets_t* _subsystem_1e7d;
	Packets_t* _block_1e7e;
	Packets_t* _main_1e7f;
	Packets_t _subsystem_1e80;
	Packets_t _block_1e89;
	Packets_t _main_1e92;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1ea2;
		CyberComposition::Simulink::Subsystem block_1ea3;
		SFC::Function main_1ea4;
		CyberComposition::Simulink::SFState state_1ea5;
	};

	std::list< Match> _matches;
};

class IsSubsystem_1eac
{
public:
	bool operator()( const Packets_t& subsystems_1ead, const Packets_t& blocks_1eb0, const Packets_t& mains_1eb3, Packets_t& subsystems_1eaf, Packets_t& blocks_1eb2, Packets_t& mains_1eb5);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1ebd, const Udm::Object& block_1ec6, const Udm::Object& main_1ecf);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1ead, const Packets_t& blocks_1eb0, const Packets_t& mains_1eb3);
	bool patternMatcher( const Udm::Object& subsystem_1ebb, const Udm::Object& block_1ec4, const Udm::Object& main_1ecd);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_1edb, const CyberComposition::Simulink::Subsystem& block_1edd, const SFC::Function& main_1edf);

private:
	Packets_t* _subsystem_1eb6;
	Packets_t* _block_1eb7;
	Packets_t* _main_1eb8;
	Packets_t _subsystem_1eb9;
	Packets_t _block_1ec2;
	Packets_t _main_1ecb;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1ed8;
		CyberComposition::Simulink::Subsystem block_1ed9;
		SFC::Function main_1eda;
	};

	std::list< Match> _matches;
};

class Otherwise_1ee1
{
public:
	bool operator()( const Packets_t& subsystems_1ee2, const Packets_t& blocks_1ee5, const Packets_t& mains_1ee8, Packets_t& subsystems_1ee4, Packets_t& blocks_1ee7, Packets_t& mains_1eea);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1ef2, const Udm::Object& block_1efb, const Udm::Object& main_1f04);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1ee2, const Packets_t& blocks_1ee5, const Packets_t& mains_1ee8);
	bool patternMatcher( const Udm::Object& subsystem_1ef0, const Udm::Object& block_1ef9, const Udm::Object& main_1f02);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_1f10, const CyberComposition::Simulink::Block& block_1f12, const SFC::Function& main_1f14);

private:
	Packets_t* _subsystem_1eeb;
	Packets_t* _block_1eec;
	Packets_t* _main_1eed;
	Packets_t _subsystem_1eee;
	Packets_t _block_1ef7;
	Packets_t _main_1f00;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1f0d;
		CyberComposition::Simulink::Block block_1f0e;
		SFC::Function main_1f0f;
	};

	std::list< Match> _matches;
};

class SubsystemChildBlock_1f16
{
public:
	void operator()( const Packets_t& systems_1f17, const Packets_t& childSubsystems_1f19, const Packets_t& systemFunctions_1f1b);

protected:
	void callMakeTrigger_2df2( const Packets_t& systems_1fa0, const Packets_t& childSubsystems_1fa2, const Packets_t& systemFunctions_1fa4);
	void callCreateMethodCall_2df6( const Packets_t& subsystems_1f1e, const Packets_t& blocks_1f22, const Packets_t& mains_1f25);
	void callCreateFunctionCallArgs_2dfa( const Packets_t& subsystems_1f62, const Packets_t& functions_1f65, const Packets_t& methodCalls_1f68);
	void callMakeArgDeclRef_2dfe( const Packets_t& systems_2cf6, const Packets_t& argPorts_2cf8, const Packets_t& argVals_2cfa);
};

class CreateMethodCall_1f1d
{
public:
	void operator()( const Packets_t& subsystems_1f1e, const Packets_t& blocks_1f22, const Packets_t& mains_1f25, Packets_t& subsystems_1f20, Packets_t& functions_1f21, Packets_t& methodCalls_1f24);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1f2e, const Udm::Object& block_1f37, const Udm::Object& main_1f40);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1f1e, const Packets_t& blocks_1f22, const Packets_t& mains_1f25);
	bool patternMatcher( const Udm::Object& subsystem_1f2c, const Udm::Object& block_1f35, const Udm::Object& main_1f3e);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_1f5b, const SFC::Function& function_1f5d, const SFC::FunctionCall& methodCall_1f5f);

private:
	Packets_t* _subsystem_1f27;
	Packets_t* _function_1f28;
	Packets_t* _methodCall_1f29;
	Packets_t _subsystem_1f2a;
	Packets_t _block_1f33;
	Packets_t _main_1f3c;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1f52;
		CyberComposition::Simulink::Subsystem block_1f53;
		SFC::CompoundStatement main_1f54;
		SFC::Function function_1f55;
		SFC::Class class_1f56;
		SFC::Arg arg_1f57;
		SFC::Struct struct_1f58;
	};

	std::list< Match> _matches;
};

class CreateFunctionCallArgs_1f61
{
public:
	void operator()( const Packets_t& subsystems_1f62, const Packets_t& functions_1f65, const Packets_t& methodCalls_1f68, Packets_t& subsystems_1f64, Packets_t& ports_1f67, Packets_t& argVals_1f6a);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1f72, const Udm::Object& function_1f7b, const Udm::Object& methodCall_1f84);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1f62, const Packets_t& functions_1f65, const Packets_t& methodCalls_1f68);
	bool patternMatcher( const Udm::Object& subsystem_1f70, const Udm::Object& function_1f79, const Udm::Object& methodCall_1f82);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_1f99, const CyberComposition::Simulink::SF_Port& port_1f9b, const SFC::ArgVal& argVal_1f9d);

private:
	Packets_t* _subsystem_1f6b;
	Packets_t* _port_1f6c;
	Packets_t* _argVal_1f6d;
	Packets_t _subsystem_1f6e;
	Packets_t _function_1f77;
	Packets_t _methodCall_1f80;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_1f93;
		SFC::Function function_1f94;
		SFC::FunctionCall methodCall_1f95;
		SFC::Arg arg_1f96;
		CyberComposition::Simulink::SF_Port port_1f97;
	};

	std::list< Match> _matches;
};

class MakeTrigger_1f9f
{
public:
	void operator()( const Packets_t& systems_1fa0, const Packets_t& childSubsystems_1fa2, const Packets_t& systemFunctions_1fa4, Packets_t& systems_1fa6, Packets_t& childSubsystems_1fa7, Packets_t& compoundStatements_1fa8);

protected:
	void callIsTriggered_2ce9( const Packets_t& systems_1fad, const Packets_t& childSubsystems_1faf, const Packets_t& systemFunctions_1fb1);
	void callCreateTriggering_2ced( const Packets_t& systems_2082, const Packets_t& childSubsystems_2084, const Packets_t& systemFunctions_2086);
	void callCreateAction_2cf1( const Packets_t& subsystems_2caa, const Packets_t& blocks_2cad, const Packets_t& functions_2cb0);

private:
	Packets_t* _system_1fa9;
	Packets_t* _childSubsystem_1faa;
	Packets_t* _compoundStatement_1fab;
};

class IsTriggered_1fac
{
public:
	void operator()( const Packets_t& systems_1fad, const Packets_t& childSubsystems_1faf, const Packets_t& systemFunctions_1fb1, Packets_t& systems_1fb3, Packets_t& childSubsystems_1fb4, Packets_t& systemFunctions_1fb5, Packets_t& systems_1fb6, Packets_t& childSubsystems_1fb7, Packets_t& systemFunctions_1fb8, Packets_t& systems_1fb9, Packets_t& childSubsystems_1fba, Packets_t& systemFunctions_1fbb);

protected:
	void executeOne( const Packets_t& systems_1fad, const Packets_t& childSubsystems_1faf, const Packets_t& systemFunctions_1fb1);
	bool isInputUnique( const Udm::Object& system_1fc7, const Udm::Object& childSubsystem_1fce, const Udm::Object& systemFunction_1fd5);

private:
	Packets_t* _system_1fbc;
	Packets_t* _childSubsystem_1fbd;
	Packets_t* _systemFunction_1fbe;
	Packets_t* _system_1fbf;
	Packets_t* _childSubsystem_1fc0;
	Packets_t* _systemFunction_1fc1;
	Packets_t* _system_1fc2;
	Packets_t* _childSubsystem_1fc3;
	Packets_t* _systemFunction_1fc4;
	Packets_t _system_1fc5;
	Packets_t _childSubsystem_1fcc;
	Packets_t _systemFunction_1fd3;
};

class HasTriggers_1fda
{
public:
	bool operator()( const Packets_t& subsystems_1fdb, const Packets_t& blocks_1fde, const Packets_t& mains_1fe1, Packets_t& subsystems_1fdd, Packets_t& blocks_1fe0, Packets_t& mains_1fe3);

protected:
	bool isInputUnique( const Udm::Object& subsystem_1feb, const Udm::Object& block_1ff4, const Udm::Object& main_1ffd);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_1fdb, const Packets_t& blocks_1fde, const Packets_t& mains_1fe1);
	bool patternMatcher( const Udm::Object& subsystem_1fe9, const Udm::Object& block_1ff2, const Udm::Object& main_1ffb);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_200d, const CyberComposition::Simulink::Subsystem& block_200f, const SFC::Function& main_2011);

private:
	Packets_t* _subsystem_1fe4;
	Packets_t* _block_1fe5;
	Packets_t* _main_1fe6;
	Packets_t _subsystem_1fe7;
	Packets_t _block_1ff0;
	Packets_t _main_1ff9;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_2009;
		CyberComposition::Simulink::Subsystem block_200a;
		SFC::Function main_200b;
		CyberComposition::Simulink::TriggerPort triggerPort_200c;
	};

	std::list< Match> _matches;
};

class HasActionPort_2013
{
public:
	bool operator()( const Packets_t& subsystems_2014, const Packets_t& blocks_2017, const Packets_t& mains_201a, Packets_t& subsystems_2016, Packets_t& blocks_2019, Packets_t& mains_201c);

protected:
	bool isInputUnique( const Udm::Object& subsystem_2024, const Udm::Object& block_202d, const Udm::Object& main_2036);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_2014, const Packets_t& blocks_2017, const Packets_t& mains_201a);
	bool patternMatcher( const Udm::Object& subsystem_2022, const Udm::Object& block_202b, const Udm::Object& main_2034);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_2046, const CyberComposition::Simulink::Subsystem& block_2048, const SFC::Function& main_204a);

private:
	Packets_t* _subsystem_201d;
	Packets_t* _block_201e;
	Packets_t* _main_201f;
	Packets_t _subsystem_2020;
	Packets_t _block_2029;
	Packets_t _main_2032;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_2042;
		CyberComposition::Simulink::Subsystem block_2043;
		SFC::Function main_2044;
		CyberComposition::Simulink::EnablePort actionPort_2045;
	};

	std::list< Match> _matches;
};

class Otherwise_204c
{
public:
	bool operator()( const Packets_t& subsystems_204d, const Packets_t& blocks_2050, const Packets_t& mains_2053, Packets_t& subsystems_204f, Packets_t& blocks_2052, Packets_t& mains_2055);

protected:
	bool isInputUnique( const Udm::Object& subsystem_205d, const Udm::Object& block_2066, const Udm::Object& main_206f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_204d, const Packets_t& blocks_2050, const Packets_t& mains_2053);
	bool patternMatcher( const Udm::Object& subsystem_205b, const Udm::Object& block_2064, const Udm::Object& main_206d);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_207b, const CyberComposition::Simulink::Subsystem& block_207d, const SFC::Function& main_207f);

private:
	Packets_t* _subsystem_2056;
	Packets_t* _block_2057;
	Packets_t* _main_2058;
	Packets_t _subsystem_2059;
	Packets_t _block_2062;
	Packets_t _main_206b;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_2078;
		CyberComposition::Simulink::Subsystem block_2079;
		SFC::Function main_207a;
	};

	std::list< Match> _matches;
};

class CreateTriggering_2081
{
public:
	void operator()( const Packets_t& systems_2082, const Packets_t& childSubsystems_2084, const Packets_t& systemFunctions_2086, Packets_t& systems_2088, Packets_t& childSubsystems_2089, Packets_t& conditionalBlocks_208a);

protected:
	void callGetTriggerPort_2c9b( const Packets_t& subsystems_20d0, const Packets_t& blocks_20d3, const Packets_t& mains_20d6);
	void callAssignCondVal_2c9f( const Packets_t& subsystems_208f, const Packets_t& triggerPorts_2092, const Packets_t& functions_2097);
	void callMakeTrigger_2ca3( const Packets_t& systems_2109, const Packets_t& childSubsystemTriggers_210b, const Packets_t& argdecls_210d, const Packets_t& dts_210f, const Packets_t& functions_2111);

private:
	Packets_t* _system_208b;
	Packets_t* _childSubsystem_208c;
	Packets_t* _conditionalBlock_208d;
};

class AssignCondVal_208e
{
public:
	void operator()( const Packets_t& subsystems_208f, const Packets_t& triggerPorts_2092, const Packets_t& functions_2097, Packets_t& subsystems_2091, Packets_t& triggerPorts_2094, Packets_t& argDeclBases_2095, Packets_t& dTs_2096, Packets_t& functions_2099);

protected:
	bool isInputUnique( const Udm::Object& subsystem_20a3, const Udm::Object& triggerPort_20ac, const Udm::Object& function_20b5);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_208f, const Packets_t& triggerPorts_2092, const Packets_t& functions_2097);
	bool patternMatcher( const Udm::Object& subsystem_20a1, const Udm::Object& triggerPort_20aa, const Udm::Object& function_20b3);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_20c5, const CyberComposition::Simulink::SF_Port& triggerPort_20c7, const SFC::ArgDeclBase& argDeclBase_20c9, const SFC::DT& dT_20cb, const SFC::Function& function_20cd);

private:
	Packets_t* _subsystem_209a;
	Packets_t* _triggerPort_209b;
	Packets_t* _argDeclBase_209c;
	Packets_t* _dT_209d;
	Packets_t* _function_209e;
	Packets_t _subsystem_209f;
	Packets_t _triggerPort_20a8;
	Packets_t _function_20b1;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_20c0;
		CyberComposition::Simulink::SF_Port triggerPort_20c1;
		SFC::Function function_20c2;
		SFC::ArgDeclBase argDeclBase_20c3;
		SFC::DT dT_20c4;
	};

	std::list< Match> _matches;
};

class GetTriggerPort_20cf
{
public:
	void operator()( const Packets_t& subsystems_20d0, const Packets_t& blocks_20d3, const Packets_t& mains_20d6, Packets_t& subsystems_20d2, Packets_t& triggerPorts_20d5, Packets_t& mains_20d8);

protected:
	bool isInputUnique( const Udm::Object& subsystem_20e0, const Udm::Object& block_20e9, const Udm::Object& main_20f2);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_20d0, const Packets_t& blocks_20d3, const Packets_t& mains_20d6);
	bool patternMatcher( const Udm::Object& subsystem_20de, const Udm::Object& block_20e7, const Udm::Object& main_20f0);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_2102, const CyberComposition::Simulink::TriggerPort& triggerPort_2104, const SFC::Function& main_2106);

private:
	Packets_t* _subsystem_20d9;
	Packets_t* _triggerPort_20da;
	Packets_t* _main_20db;
	Packets_t _subsystem_20dc;
	Packets_t _block_20e5;
	Packets_t _main_20ee;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_20fe;
		CyberComposition::Simulink::Block block_20ff;
		SFC::Function main_2100;
		CyberComposition::Simulink::TriggerPort triggerPort_2101;
	};

	std::list< Match> _matches;
};

class MakeTrigger_2108
{
public:
	void operator()( const Packets_t& systems_2109, const Packets_t& childSubsystemTriggers_210b, const Packets_t& argdecls_210d, const Packets_t& dts_210f, const Packets_t& functions_2111, Packets_t& systems_2113, Packets_t& childSubsystems_2114, Packets_t& conditionals_2115);

protected:
	void callAddStateVar_2c7f( const Packets_t& subsystems_2900, const Packets_t& triggerPorts_2903, const Packets_t& argDeclBases_2906, const Packets_t& dTs_290a, const Packets_t& functions_290e);
	void callAddTriggerVars_2c85( const Packets_t& systems_211a, const Packets_t& triggers_211c, const Packets_t& argdecls_211e, const Packets_t& dts_2120, const Packets_t& oldvals_2122, const Packets_t& contexts_2124, const Packets_t& functions_2126);
	void callSaveState_2c8d( const Packets_t& systems_2814, const Packets_t& triggers_2816, const Packets_t& argdecls_2818, const Packets_t& dts_281a, const Packets_t& oldvals_281c, const Packets_t& contexts_281e, const Packets_t& functions_2820);
	void callMakeCondition_2c95( const Packets_t& systems_296b, const Packets_t& triggers_296d, const Packets_t& dts_296f, const Packets_t& oldvals_2971, const Packets_t& functions_2973);

private:
	Packets_t* _system_2116;
	Packets_t* _childSubsystem_2117;
	Packets_t* _conditional_2118;
};

class AddTriggerVars_2119
{
public:
	void operator()( const Packets_t& systems_211a, const Packets_t& triggers_211c, const Packets_t& argdecls_211e, const Packets_t& dts_2120, const Packets_t& oldvals_2122, const Packets_t& contexts_2124, const Packets_t& functions_2126, Packets_t& systems_2128, Packets_t& triggers_2129, Packets_t& argdecls_212a, Packets_t& dts_212b, Packets_t& oldvals_212c, Packets_t& contexts_212d, Packets_t& functions_212e);

protected:
	void callAddMatrixTriggerVars_2805( const Packets_t& triggers_2137, const Packets_t& argdecls_2139, const Packets_t& dts_213b, const Packets_t& oldvals_213d, const Packets_t& contexts_213f, const Packets_t& functions_2141);
	void callAddStructTriggerVars_280c( const Packets_t& triggers_245e, const Packets_t& argdecls_2460, const Packets_t& dts_2462, const Packets_t& oldvals_2464, const Packets_t& contexts_2466, const Packets_t& functions_2468);

private:
	Packets_t* _system_212f;
	Packets_t* _trigger_2130;
	Packets_t* _argdecl_2131;
	Packets_t* _dt_2132;
	Packets_t* _oldval_2133;
	Packets_t* _context_2134;
	Packets_t* _function_2135;
};

class AddMatrixTriggerVars_2136
{
public:
	void operator()( const Packets_t& triggers_2137, const Packets_t& argdecls_2139, const Packets_t& dts_213b, const Packets_t& oldvals_213d, const Packets_t& contexts_213f, const Packets_t& functions_2141, Packets_t& triggers_2143, Packets_t& argdecls_2144, Packets_t& dts_2145, Packets_t& oldvals_2146, Packets_t& contexts_2147, Packets_t& functions_2148);

protected:
	void callAddTriggerVar_2430( const Packets_t& triggerPorts_23c5, const Packets_t& argDeclBases_23c8, const Packets_t& dTs_23cb, const Packets_t& localVars_23cd, const Packets_t& contexts_23d0, const Packets_t& functions_23d4);
	void callStartAssignment_2437( const Packets_t& triggerPorts_2360, const Packets_t& argDeclBases_2363, const Packets_t& oldvals_2366, const Packets_t& contexts_2369, const Packets_t& triggerVars_236c, const Packets_t& functions_236e);
	void callEdgeTest_243e( const Packets_t& triggers_2218, const Packets_t& argdecls_221a, const Packets_t& oldvals_221c, const Packets_t& contexts_221e, const Packets_t& initexprs_2220);
	void callEitherEdge_2444( const Packets_t& argDeclBases_2150, const Packets_t& localVars_2153, const Packets_t& contexts_2156, const Packets_t& unaryExprss_2159);
	void callFallingEdge_2449( const Packets_t& argDeclBases_219c, const Packets_t& localVars_219e, const Packets_t& contexts_21a0, const Packets_t& unaryExprss_21a2);
	void callRisingEdge_244e( const Packets_t& argDeclBases_21da, const Packets_t& localVars_21dc, const Packets_t& contexts_21de, const Packets_t& unaryExprss_21e0);
	void callFallingEdge_2453( const Packets_t& argDeclBases_219c, const Packets_t& localVars_219e, const Packets_t& contexts_21a0, const Packets_t& unaryExprss_21a2);
	void callRisingEdge_2458( const Packets_t& argDeclBases_21da, const Packets_t& localVars_21dc, const Packets_t& contexts_21de, const Packets_t& unaryExprss_21e0);

private:
	Packets_t* _trigger_2149;
	Packets_t* _argdecl_214a;
	Packets_t* _dt_214b;
	Packets_t* _oldval_214c;
	Packets_t* _context_214d;
	Packets_t* _function_214e;
};

class EitherEdge_214f
{
public:
	void operator()( const Packets_t& argDeclBases_2150, const Packets_t& localVars_2153, const Packets_t& contexts_2156, const Packets_t& unaryExprss_2159, Packets_t& argDeclBases_2152, Packets_t& localVars_2155, Packets_t& contexts_2158, Packets_t& leftUnaryExprss_215b, Packets_t& rightUnaryExprss_215c);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_2166, const Udm::Object& localVar_216f, const Udm::Object& context_2178, const Udm::Object& unaryExprs_2181);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_2150, const Packets_t& localVars_2153, const Packets_t& contexts_2156, const Packets_t& unaryExprss_2159);
	bool patternMatcher( const Udm::Object& argDeclBase_2164, const Udm::Object& localVar_216d, const Udm::Object& context_2176, const Udm::Object& unaryExprs_217f);
	void effector();
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_2191, const SFC::LocalVar& localVar_2193, const SFC::Arg& context_2195, const SFC::UnaryExprs& leftUnaryExprs_2197, const SFC::UnaryExprs& rightUnaryExprs_2199);

private:
	Packets_t* _argDeclBase_215d;
	Packets_t* _localVar_215e;
	Packets_t* _context_215f;
	Packets_t* _leftUnaryExprs_2160;
	Packets_t* _rightUnaryExprs_2161;
	Packets_t _argDeclBase_2162;
	Packets_t _localVar_216b;
	Packets_t _context_2174;
	Packets_t _unaryExprs_217d;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_218a;
		SFC::LocalVar localVar_218b;
		SFC::Arg context_218c;
		SFC::UnaryExprs unaryExprs_218d;
	};

	std::list< Match> _matches;
};

class FallingEdge_219b
{
public:
	void operator()( const Packets_t& argDeclBases_219c, const Packets_t& localVars_219e, const Packets_t& contexts_21a0, const Packets_t& unaryExprss_21a2);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_21a8, const Udm::Object& localVar_21b1, const Udm::Object& context_21ba, const Udm::Object& unaryExprs_21c3);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_219c, const Packets_t& localVars_219e, const Packets_t& contexts_21a0, const Packets_t& unaryExprss_21a2);
	bool patternMatcher( const Udm::Object& argDeclBase_21a6, const Udm::Object& localVar_21af, const Udm::Object& context_21b8, const Udm::Object& unaryExprs_21c1);
	void effector();

private:
	Packets_t _argDeclBase_21a4;
	Packets_t _localVar_21ad;
	Packets_t _context_21b6;
	Packets_t _unaryExprs_21bf;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_21cc;
		SFC::LocalVar localVar_21cd;
		SFC::Arg context_21ce;
		SFC::UnaryExprs unaryExprs_21cf;
	};

	std::list< Match> _matches;
};

class RisingEdge_21d9
{
public:
	void operator()( const Packets_t& argDeclBases_21da, const Packets_t& localVars_21dc, const Packets_t& contexts_21de, const Packets_t& unaryExprss_21e0);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_21e6, const Udm::Object& localVar_21ef, const Udm::Object& context_21f8, const Udm::Object& unaryExprs_2201);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_21da, const Packets_t& localVars_21dc, const Packets_t& contexts_21de, const Packets_t& unaryExprss_21e0);
	bool patternMatcher( const Udm::Object& argDeclBase_21e4, const Udm::Object& localVar_21ed, const Udm::Object& context_21f6, const Udm::Object& unaryExprs_21ff);
	void effector();

private:
	Packets_t _argDeclBase_21e2;
	Packets_t _localVar_21eb;
	Packets_t _context_21f4;
	Packets_t _unaryExprs_21fd;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_220a;
		SFC::LocalVar localVar_220b;
		SFC::Arg context_220c;
		SFC::UnaryExprs unaryExprs_220d;
	};

	std::list< Match> _matches;
};

class EdgeTest_2217
{
public:
	void operator()( const Packets_t& triggers_2218, const Packets_t& argdecls_221a, const Packets_t& oldvals_221c, const Packets_t& contexts_221e, const Packets_t& initexprs_2220, Packets_t& argdecls_2222, Packets_t& oldvals_2223, Packets_t& contexts_2224, Packets_t& initexprs_2225, Packets_t& argdecls_2226, Packets_t& oldvals_2227, Packets_t& contexts_2228, Packets_t& initexprs_2229, Packets_t& argdecls_222a, Packets_t& oldvals_222b, Packets_t& contexts_222c, Packets_t& initexprs_222d);

protected:
	void executeOne( const Packets_t& triggers_2218, const Packets_t& argdecls_221a, const Packets_t& oldvals_221c, const Packets_t& contexts_221e, const Packets_t& initexprs_2220);
	bool isInputUnique( const Udm::Object& trigger_223c, const Udm::Object& argdecl_2243, const Udm::Object& oldval_224a, const Udm::Object& context_2251, const Udm::Object& initexpr_2258);

private:
	Packets_t* _argdecl_222e;
	Packets_t* _oldval_222f;
	Packets_t* _context_2230;
	Packets_t* _initexpr_2231;
	Packets_t* _argdecl_2232;
	Packets_t* _oldval_2233;
	Packets_t* _context_2234;
	Packets_t* _initexpr_2235;
	Packets_t* _argdecl_2236;
	Packets_t* _oldval_2237;
	Packets_t* _context_2238;
	Packets_t* _initexpr_2239;
	Packets_t _trigger_223a;
	Packets_t _argdecl_2241;
	Packets_t _oldval_2248;
	Packets_t _context_224f;
	Packets_t _initexpr_2256;
};

class RisingEdge_225d
{
public:
	bool operator()( const Packets_t& triggerPorts_225e, const Packets_t& argDeclBases_2260, const Packets_t& localVars_2263, const Packets_t& contexts_2266, const Packets_t& unaryExprss_2269, Packets_t& argDeclBases_2262, Packets_t& localVars_2265, Packets_t& contexts_2268, Packets_t& unaryExprss_226b);

protected:
	bool isInputUnique( const Udm::Object& triggerPort_2274, const Udm::Object& argDeclBase_227d, const Udm::Object& localVar_2286, const Udm::Object& context_228f, const Udm::Object& unaryExprs_2298);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::Arg& Context, SFC::LocalVar& LocalVar, CyberComposition::Simulink::TriggerPort& TriggerPort, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& triggerPorts_225e, const Packets_t& argDeclBases_2260, const Packets_t& localVars_2263, const Packets_t& contexts_2266, const Packets_t& unaryExprss_2269);
	bool patternMatcher( const Udm::Object& triggerPort_2272, const Udm::Object& argDeclBase_227b, const Udm::Object& localVar_2284, const Udm::Object& context_228d, const Udm::Object& unaryExprs_2296);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_22ab, const SFC::LocalVar& localVar_22ad, const SFC::Arg& context_22af, const SFC::UnaryExprs& unaryExprs_22b1);

private:
	Packets_t* _argDeclBase_226c;
	Packets_t* _localVar_226d;
	Packets_t* _context_226e;
	Packets_t* _unaryExprs_226f;
	Packets_t _triggerPort_2270;
	Packets_t _argDeclBase_2279;
	Packets_t _localVar_2282;
	Packets_t _context_228b;
	Packets_t _unaryExprs_2294;
	class Match
	{
	public:
		CyberComposition::Simulink::TriggerPort triggerPort_22a1;
		SFC::ArgDeclBase argDeclBase_22a2;
		SFC::LocalVar localVar_22a3;
		SFC::Arg context_22a4;
		SFC::UnaryExprs unaryExprs_22a5;
	};

	std::list< Match> _matches;
};

class FallingEdge_22b3
{
public:
	bool operator()( const Packets_t& triggerPorts_22b4, const Packets_t& argDeclBases_22b6, const Packets_t& localVars_22b9, const Packets_t& contexts_22bc, const Packets_t& unaryExprss_22bf, Packets_t& argDeclBases_22b8, Packets_t& localVars_22bb, Packets_t& contexts_22be, Packets_t& unaryExprss_22c1);

protected:
	bool isInputUnique( const Udm::Object& triggerPort_22ca, const Udm::Object& argDeclBase_22d3, const Udm::Object& localVar_22dc, const Udm::Object& context_22e5, const Udm::Object& unaryExprs_22ee);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::Arg& Context, SFC::LocalVar& LocalVar, CyberComposition::Simulink::TriggerPort& TriggerPort, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& triggerPorts_22b4, const Packets_t& argDeclBases_22b6, const Packets_t& localVars_22b9, const Packets_t& contexts_22bc, const Packets_t& unaryExprss_22bf);
	bool patternMatcher( const Udm::Object& triggerPort_22c8, const Udm::Object& argDeclBase_22d1, const Udm::Object& localVar_22da, const Udm::Object& context_22e3, const Udm::Object& unaryExprs_22ec);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_2301, const SFC::LocalVar& localVar_2303, const SFC::Arg& context_2305, const SFC::UnaryExprs& unaryExprs_2307);

private:
	Packets_t* _argDeclBase_22c2;
	Packets_t* _localVar_22c3;
	Packets_t* _context_22c4;
	Packets_t* _unaryExprs_22c5;
	Packets_t _triggerPort_22c6;
	Packets_t _argDeclBase_22cf;
	Packets_t _localVar_22d8;
	Packets_t _context_22e1;
	Packets_t _unaryExprs_22ea;
	class Match
	{
	public:
		CyberComposition::Simulink::TriggerPort triggerPort_22f7;
		SFC::ArgDeclBase argDeclBase_22f8;
		SFC::LocalVar localVar_22f9;
		SFC::Arg context_22fa;
		SFC::UnaryExprs unaryExprs_22fb;
	};

	std::list< Match> _matches;
};

class EitherEdge_2309
{
public:
	bool operator()( const Packets_t& triggerPorts_230a, const Packets_t& argDeclBases_230c, const Packets_t& localVars_230f, const Packets_t& contexts_2312, const Packets_t& unaryExprss_2315, Packets_t& argDeclBases_230e, Packets_t& localVars_2311, Packets_t& contexts_2314, Packets_t& unaryExprss_2317);

protected:
	bool isInputUnique( const Udm::Object& triggerPort_2320, const Udm::Object& argDeclBase_2329, const Udm::Object& localVar_2332, const Udm::Object& context_233b, const Udm::Object& unaryExprs_2344);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::Arg& Context, SFC::LocalVar& LocalVar, CyberComposition::Simulink::TriggerPort& TriggerPort, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& triggerPorts_230a, const Packets_t& argDeclBases_230c, const Packets_t& localVars_230f, const Packets_t& contexts_2312, const Packets_t& unaryExprss_2315);
	bool patternMatcher( const Udm::Object& triggerPort_231e, const Udm::Object& argDeclBase_2327, const Udm::Object& localVar_2330, const Udm::Object& context_2339, const Udm::Object& unaryExprs_2342);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_2357, const SFC::LocalVar& localVar_2359, const SFC::Arg& context_235b, const SFC::UnaryExprs& unaryExprs_235d);

private:
	Packets_t* _argDeclBase_2318;
	Packets_t* _localVar_2319;
	Packets_t* _context_231a;
	Packets_t* _unaryExprs_231b;
	Packets_t _triggerPort_231c;
	Packets_t _argDeclBase_2325;
	Packets_t _localVar_232e;
	Packets_t _context_2337;
	Packets_t _unaryExprs_2340;
	class Match
	{
	public:
		CyberComposition::Simulink::TriggerPort triggerPort_234d;
		SFC::ArgDeclBase argDeclBase_234e;
		SFC::LocalVar localVar_234f;
		SFC::Arg context_2350;
		SFC::UnaryExprs unaryExprs_2351;
	};

	std::list< Match> _matches;
};

class StartAssignment_235f
{
public:
	void operator()( const Packets_t& triggerPorts_2360, const Packets_t& argDeclBases_2363, const Packets_t& oldvals_2366, const Packets_t& contexts_2369, const Packets_t& triggerVars_236c, const Packets_t& functions_236e, Packets_t& triggerPorts_2362, Packets_t& argDeclBases_2365, Packets_t& oldvals_2368, Packets_t& contexts_236b, Packets_t& unaryExprss_2370);

protected:
	bool isInputUnique( const Udm::Object& triggerPort_237a, const Udm::Object& argDeclBase_2383, const Udm::Object& oldval_238c, const Udm::Object& context_2395, const Udm::Object& triggerVar_239e, const Udm::Object& function_23a7);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& triggerPorts_2360, const Packets_t& argDeclBases_2363, const Packets_t& oldvals_2366, const Packets_t& contexts_2369, const Packets_t& triggerVars_236c, const Packets_t& functions_236e);
	bool patternMatcher( const Udm::Object& triggerPort_2378, const Udm::Object& argDeclBase_2381, const Udm::Object& oldval_238a, const Udm::Object& context_2393, const Udm::Object& triggerVar_239c, const Udm::Object& function_23a5);
	void effector();
	void outputAppender( const CyberComposition::Simulink::TriggerPort& triggerPort_23ba, const SFC::ArgDeclBase& argDeclBase_23bc, const SFC::LocalVar& oldval_23be, const SFC::Arg& context_23c0, const SFC::UnaryExprs& unaryExprs_23c2);

private:
	Packets_t* _triggerPort_2371;
	Packets_t* _argDeclBase_2372;
	Packets_t* _oldval_2373;
	Packets_t* _context_2374;
	Packets_t* _unaryExprs_2375;
	Packets_t _triggerPort_2376;
	Packets_t _argDeclBase_237f;
	Packets_t _oldval_2388;
	Packets_t _context_2391;
	Packets_t _triggerVar_239a;
	Packets_t _function_23a3;
	class Match
	{
	public:
		CyberComposition::Simulink::TriggerPort triggerPort_23b0;
		SFC::ArgDeclBase argDeclBase_23b1;
		SFC::LocalVar oldval_23b2;
		SFC::Arg context_23b3;
		SFC::LocalVar triggerVar_23b4;
		SFC::Function function_23b5;
	};

	std::list< Match> _matches;
};

class AddTriggerVar_23c4
{
public:
	void operator()( const Packets_t& triggerPorts_23c5, const Packets_t& argDeclBases_23c8, const Packets_t& dTs_23cb, const Packets_t& localVars_23cd, const Packets_t& contexts_23d0, const Packets_t& functions_23d4, Packets_t& triggerPorts_23c7, Packets_t& argDeclBases_23ca, Packets_t& localVars_23cf, Packets_t& contexts_23d2, Packets_t& triggerVars_23d3, Packets_t& functions_23d6);

protected:
	bool isInputUnique( const Udm::Object& triggerPort_23e1, const Udm::Object& argDeclBase_23ea, const Udm::Object& dT_23f3, const Udm::Object& localVar_23fc, const Udm::Object& context_2405, const Udm::Object& function_240e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::Arg& Context, SFC::DT& DT, SFC::Function& Function, SFC::LocalVar& LocalVar, CyberComposition::Simulink::TriggerPort& TriggerPort);
	void processInputPackets( const Packets_t& triggerPorts_23c5, const Packets_t& argDeclBases_23c8, const Packets_t& dTs_23cb, const Packets_t& localVars_23cd, const Packets_t& contexts_23d0, const Packets_t& functions_23d4);
	bool patternMatcher( const Udm::Object& triggerPort_23df, const Udm::Object& argDeclBase_23e8, const Udm::Object& dT_23f1, const Udm::Object& localVar_23fa, const Udm::Object& context_2403, const Udm::Object& function_240c);
	void effector();
	void outputAppender( const CyberComposition::Simulink::TriggerPort& triggerPort_2424, const SFC::ArgDeclBase& argDeclBase_2426, const SFC::LocalVar& localVar_2428, const SFC::Arg& context_242a, const SFC::LocalVar& triggerVar_242c, const SFC::Function& function_242e);

private:
	Packets_t* _triggerPort_23d7;
	Packets_t* _argDeclBase_23d8;
	Packets_t* _localVar_23d9;
	Packets_t* _context_23da;
	Packets_t* _triggerVar_23db;
	Packets_t* _function_23dc;
	Packets_t _triggerPort_23dd;
	Packets_t _argDeclBase_23e6;
	Packets_t _dT_23ef;
	Packets_t _localVar_23f8;
	Packets_t _context_2401;
	Packets_t _function_240a;
	class Match
	{
	public:
		CyberComposition::Simulink::TriggerPort triggerPort_2417;
		SFC::ArgDeclBase argDeclBase_2418;
		SFC::DT dT_2419;
		SFC::LocalVar localVar_241a;
		SFC::Arg context_241b;
		SFC::Function function_241c;
	};

	std::list< Match> _matches;
};

class AddStructTriggerVars_245d
{
public:
	void operator()( const Packets_t& triggers_245e, const Packets_t& argdecls_2460, const Packets_t& dts_2462, const Packets_t& oldvals_2464, const Packets_t& contexts_2466, const Packets_t& functions_2468);

protected:
	void callAddTriggerVar_27d1( const Packets_t& triggerPorts_26ed, const Packets_t& argDeclBases_26f0, const Packets_t& structs_26f3, const Packets_t& localVars_26f6, const Packets_t& contexts_26f9, const Packets_t& functions_26fd);
	void callStartAssignment_27d8( const Packets_t& triggerPorts_2757, const Packets_t& argDeclBases_275a, const Packets_t& structs_275e, const Packets_t& oldvals_2760, const Packets_t& contexts_2763, const Packets_t& triggerVars_2766, const Packets_t& functions_2768);
	void callEdgeTest_27e0( const Packets_t& triggers_2563, const Packets_t& argdecls_2565, const Packets_t& members_2567, const Packets_t& oldvals_2569, const Packets_t& contexts_256b, const Packets_t& initexprs_256d);
	void callEitherEdge_27e7( const Packets_t& argDeclBases_246b, const Packets_t& members_246e, const Packets_t& localVars_2471, const Packets_t& contexts_2474, const Packets_t& unaryExprss_2477);
	void callFallingEdge_27ed( const Packets_t& argDeclBases_24c7, const Packets_t& members_24c9, const Packets_t& localVars_24cb, const Packets_t& contexts_24cd, const Packets_t& unaryExprss_24cf);
	void callRisingEdge_27f3( const Packets_t& argDeclBases_2515, const Packets_t& members_2517, const Packets_t& localVars_2519, const Packets_t& contexts_251b, const Packets_t& unaryExprss_251d);
	void callFallingEdge_27f9( const Packets_t& argDeclBases_24c7, const Packets_t& members_24c9, const Packets_t& localVars_24cb, const Packets_t& contexts_24cd, const Packets_t& unaryExprss_24cf);
	void callRisingEdge_27ff( const Packets_t& argDeclBases_2515, const Packets_t& members_2517, const Packets_t& localVars_2519, const Packets_t& contexts_251b, const Packets_t& unaryExprss_251d);
};

class EitherEdge_246a
{
public:
	void operator()( const Packets_t& argDeclBases_246b, const Packets_t& members_246e, const Packets_t& localVars_2471, const Packets_t& contexts_2474, const Packets_t& unaryExprss_2477, Packets_t& argDeclBases_246d, Packets_t& members_2470, Packets_t& localVars_2473, Packets_t& contexts_2476, Packets_t& leftUnaryExprss_2479, Packets_t& rightUnaryExprss_247a);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_2485, const Udm::Object& member_248e, const Udm::Object& localVar_2497, const Udm::Object& context_24a0, const Udm::Object& unaryExprs_24a9);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_246b, const Packets_t& members_246e, const Packets_t& localVars_2471, const Packets_t& contexts_2474, const Packets_t& unaryExprss_2477);
	bool patternMatcher( const Udm::Object& argDeclBase_2483, const Udm::Object& member_248c, const Udm::Object& localVar_2495, const Udm::Object& context_249e, const Udm::Object& unaryExprs_24a7);
	void effector();
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_24ba, const SFC::LocalVar& member_24bc, const SFC::LocalVar& localVar_24be, const SFC::Arg& context_24c0, const SFC::UnaryExprs& leftUnaryExprs_24c2, const SFC::UnaryExprs& rightUnaryExprs_24c4);

private:
	Packets_t* _argDeclBase_247b;
	Packets_t* _member_247c;
	Packets_t* _localVar_247d;
	Packets_t* _context_247e;
	Packets_t* _leftUnaryExprs_247f;
	Packets_t* _rightUnaryExprs_2480;
	Packets_t _argDeclBase_2481;
	Packets_t _member_248a;
	Packets_t _localVar_2493;
	Packets_t _context_249c;
	Packets_t _unaryExprs_24a5;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_24b2;
		SFC::LocalVar member_24b3;
		SFC::LocalVar localVar_24b4;
		SFC::Arg context_24b5;
		SFC::UnaryExprs unaryExprs_24b6;
	};

	std::list< Match> _matches;
};

class FallingEdge_24c6
{
public:
	void operator()( const Packets_t& argDeclBases_24c7, const Packets_t& members_24c9, const Packets_t& localVars_24cb, const Packets_t& contexts_24cd, const Packets_t& unaryExprss_24cf);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_24d5, const Udm::Object& member_24de, const Udm::Object& localVar_24e7, const Udm::Object& context_24f0, const Udm::Object& unaryExprs_24f9);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_24c7, const Packets_t& members_24c9, const Packets_t& localVars_24cb, const Packets_t& contexts_24cd, const Packets_t& unaryExprss_24cf);
	bool patternMatcher( const Udm::Object& argDeclBase_24d3, const Udm::Object& member_24dc, const Udm::Object& localVar_24e5, const Udm::Object& context_24ee, const Udm::Object& unaryExprs_24f7);
	void effector();

private:
	Packets_t _argDeclBase_24d1;
	Packets_t _member_24da;
	Packets_t _localVar_24e3;
	Packets_t _context_24ec;
	Packets_t _unaryExprs_24f5;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_2502;
		SFC::LocalVar member_2503;
		SFC::LocalVar localVar_2504;
		SFC::Arg context_2505;
		SFC::UnaryExprs unaryExprs_2506;
	};

	std::list< Match> _matches;
};

class RisingEdge_2514
{
public:
	void operator()( const Packets_t& argDeclBases_2515, const Packets_t& members_2517, const Packets_t& localVars_2519, const Packets_t& contexts_251b, const Packets_t& unaryExprss_251d);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_2523, const Udm::Object& member_252c, const Udm::Object& localVar_2535, const Udm::Object& context_253e, const Udm::Object& unaryExprs_2547);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_2515, const Packets_t& members_2517, const Packets_t& localVars_2519, const Packets_t& contexts_251b, const Packets_t& unaryExprss_251d);
	bool patternMatcher( const Udm::Object& argDeclBase_2521, const Udm::Object& member_252a, const Udm::Object& localVar_2533, const Udm::Object& context_253c, const Udm::Object& unaryExprs_2545);
	void effector();

private:
	Packets_t _argDeclBase_251f;
	Packets_t _member_2528;
	Packets_t _localVar_2531;
	Packets_t _context_253a;
	Packets_t _unaryExprs_2543;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_2550;
		SFC::LocalVar member_2551;
		SFC::LocalVar localVar_2552;
		SFC::Arg context_2553;
		SFC::UnaryExprs unaryExprs_2554;
	};

	std::list< Match> _matches;
};

class EdgeTest_2562
{
public:
	void operator()( const Packets_t& triggers_2563, const Packets_t& argdecls_2565, const Packets_t& members_2567, const Packets_t& oldvals_2569, const Packets_t& contexts_256b, const Packets_t& initexprs_256d, Packets_t& argdecls_256f, Packets_t& members_2570, Packets_t& oldvals_2571, Packets_t& contexts_2572, Packets_t& initexprs_2573, Packets_t& argdecls_2574, Packets_t& members_2575, Packets_t& oldvals_2576, Packets_t& contexts_2577, Packets_t& initexprs_2578, Packets_t& argdecls_2579, Packets_t& members_257a, Packets_t& oldvals_257b, Packets_t& contexts_257c, Packets_t& initexprs_257d);

protected:
	void executeOne( const Packets_t& triggers_2563, const Packets_t& argdecls_2565, const Packets_t& members_2567, const Packets_t& oldvals_2569, const Packets_t& contexts_256b, const Packets_t& initexprs_256d);
	bool isInputUnique( const Udm::Object& trigger_258f, const Udm::Object& argdecl_2596, const Udm::Object& member_259d, const Udm::Object& oldval_25a4, const Udm::Object& context_25ab, const Udm::Object& initexpr_25b2);

private:
	Packets_t* _argdecl_257e;
	Packets_t* _member_257f;
	Packets_t* _oldval_2580;
	Packets_t* _context_2581;
	Packets_t* _initexpr_2582;
	Packets_t* _argdecl_2583;
	Packets_t* _member_2584;
	Packets_t* _oldval_2585;
	Packets_t* _context_2586;
	Packets_t* _initexpr_2587;
	Packets_t* _argdecl_2588;
	Packets_t* _member_2589;
	Packets_t* _oldval_258a;
	Packets_t* _context_258b;
	Packets_t* _initexpr_258c;
	Packets_t _trigger_258d;
	Packets_t _argdecl_2594;
	Packets_t _member_259b;
	Packets_t _oldval_25a2;
	Packets_t _context_25a9;
	Packets_t _initexpr_25b0;
};

class RisingEdge_25b7
{
public:
	bool operator()( const Packets_t& triggerPorts_25b8, const Packets_t& argDeclBases_25ba, const Packets_t& members_25bd, const Packets_t& localVars_25c0, const Packets_t& contexts_25c3, const Packets_t& unaryExprss_25c6, Packets_t& argDeclBases_25bc, Packets_t& members_25bf, Packets_t& localVars_25c2, Packets_t& contexts_25c5, Packets_t& unaryExprss_25c8);

protected:
	bool isInputUnique( const Udm::Object& triggerPort_25d2, const Udm::Object& argDeclBase_25db, const Udm::Object& member_25e4, const Udm::Object& localVar_25ed, const Udm::Object& context_25f6, const Udm::Object& unaryExprs_25ff);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::Arg& Context, SFC::LocalVar& LocalVar, SFC::LocalVar& Member, CyberComposition::Simulink::TriggerPort& TriggerPort, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& triggerPorts_25b8, const Packets_t& argDeclBases_25ba, const Packets_t& members_25bd, const Packets_t& localVars_25c0, const Packets_t& contexts_25c3, const Packets_t& unaryExprss_25c6);
	bool patternMatcher( const Udm::Object& triggerPort_25d0, const Udm::Object& argDeclBase_25d9, const Udm::Object& member_25e2, const Udm::Object& localVar_25eb, const Udm::Object& context_25f4, const Udm::Object& unaryExprs_25fd);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_2614, const SFC::LocalVar& member_2616, const SFC::LocalVar& localVar_2618, const SFC::Arg& context_261a, const SFC::UnaryExprs& unaryExprs_261c);

private:
	Packets_t* _argDeclBase_25c9;
	Packets_t* _member_25ca;
	Packets_t* _localVar_25cb;
	Packets_t* _context_25cc;
	Packets_t* _unaryExprs_25cd;
	Packets_t _triggerPort_25ce;
	Packets_t _argDeclBase_25d7;
	Packets_t _member_25e0;
	Packets_t _localVar_25e9;
	Packets_t _context_25f2;
	Packets_t _unaryExprs_25fb;
	class Match
	{
	public:
		CyberComposition::Simulink::TriggerPort triggerPort_2608;
		SFC::ArgDeclBase argDeclBase_2609;
		SFC::LocalVar member_260a;
		SFC::LocalVar localVar_260b;
		SFC::Arg context_260c;
		SFC::UnaryExprs unaryExprs_260d;
	};

	std::list< Match> _matches;
};

class FallingEdge_261e
{
public:
	bool operator()( const Packets_t& triggerPorts_261f, const Packets_t& argDeclBases_2621, const Packets_t& members_2624, const Packets_t& localVars_2627, const Packets_t& contexts_262a, const Packets_t& unaryExprss_262d, Packets_t& argDeclBases_2623, Packets_t& members_2626, Packets_t& localVars_2629, Packets_t& contexts_262c, Packets_t& unaryExprss_262f);

protected:
	bool isInputUnique( const Udm::Object& triggerPort_2639, const Udm::Object& argDeclBase_2642, const Udm::Object& member_264b, const Udm::Object& localVar_2654, const Udm::Object& context_265d, const Udm::Object& unaryExprs_2666);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::Arg& Context, SFC::LocalVar& LocalVar, SFC::LocalVar& Member, CyberComposition::Simulink::TriggerPort& TriggerPort, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& triggerPorts_261f, const Packets_t& argDeclBases_2621, const Packets_t& members_2624, const Packets_t& localVars_2627, const Packets_t& contexts_262a, const Packets_t& unaryExprss_262d);
	bool patternMatcher( const Udm::Object& triggerPort_2637, const Udm::Object& argDeclBase_2640, const Udm::Object& member_2649, const Udm::Object& localVar_2652, const Udm::Object& context_265b, const Udm::Object& unaryExprs_2664);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_267b, const SFC::LocalVar& member_267d, const SFC::LocalVar& localVar_267f, const SFC::Arg& context_2681, const SFC::UnaryExprs& unaryExprs_2683);

private:
	Packets_t* _argDeclBase_2630;
	Packets_t* _member_2631;
	Packets_t* _localVar_2632;
	Packets_t* _context_2633;
	Packets_t* _unaryExprs_2634;
	Packets_t _triggerPort_2635;
	Packets_t _argDeclBase_263e;
	Packets_t _member_2647;
	Packets_t _localVar_2650;
	Packets_t _context_2659;
	Packets_t _unaryExprs_2662;
	class Match
	{
	public:
		CyberComposition::Simulink::TriggerPort triggerPort_266f;
		SFC::ArgDeclBase argDeclBase_2670;
		SFC::LocalVar member_2671;
		SFC::LocalVar localVar_2672;
		SFC::Arg context_2673;
		SFC::UnaryExprs unaryExprs_2674;
	};

	std::list< Match> _matches;
};

class EitherEdge_2685
{
public:
	bool operator()( const Packets_t& triggerPorts_2686, const Packets_t& argDeclBases_2688, const Packets_t& members_268b, const Packets_t& localVars_268e, const Packets_t& contexts_2691, const Packets_t& unaryExprss_2694, Packets_t& argDeclBases_268a, Packets_t& members_268d, Packets_t& localVars_2690, Packets_t& contexts_2693, Packets_t& unaryExprss_2696);

protected:
	bool isInputUnique( const Udm::Object& triggerPort_26a0, const Udm::Object& argDeclBase_26a9, const Udm::Object& member_26b2, const Udm::Object& localVar_26bb, const Udm::Object& context_26c4, const Udm::Object& unaryExprs_26cd);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::Arg& Context, SFC::LocalVar& LocalVar, SFC::LocalVar& Member, CyberComposition::Simulink::TriggerPort& TriggerPort, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& triggerPorts_2686, const Packets_t& argDeclBases_2688, const Packets_t& members_268b, const Packets_t& localVars_268e, const Packets_t& contexts_2691, const Packets_t& unaryExprss_2694);
	bool patternMatcher( const Udm::Object& triggerPort_269e, const Udm::Object& argDeclBase_26a7, const Udm::Object& member_26b0, const Udm::Object& localVar_26b9, const Udm::Object& context_26c2, const Udm::Object& unaryExprs_26cb);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_26e2, const SFC::LocalVar& member_26e4, const SFC::LocalVar& localVar_26e6, const SFC::Arg& context_26e8, const SFC::UnaryExprs& unaryExprs_26ea);

private:
	Packets_t* _argDeclBase_2697;
	Packets_t* _member_2698;
	Packets_t* _localVar_2699;
	Packets_t* _context_269a;
	Packets_t* _unaryExprs_269b;
	Packets_t _triggerPort_269c;
	Packets_t _argDeclBase_26a5;
	Packets_t _member_26ae;
	Packets_t _localVar_26b7;
	Packets_t _context_26c0;
	Packets_t _unaryExprs_26c9;
	class Match
	{
	public:
		CyberComposition::Simulink::TriggerPort triggerPort_26d6;
		SFC::ArgDeclBase argDeclBase_26d7;
		SFC::LocalVar member_26d8;
		SFC::LocalVar localVar_26d9;
		SFC::Arg context_26da;
		SFC::UnaryExprs unaryExprs_26db;
	};

	std::list< Match> _matches;
};

class AddTriggerVar_26ec
{
public:
	void operator()( const Packets_t& triggerPorts_26ed, const Packets_t& argDeclBases_26f0, const Packets_t& structs_26f3, const Packets_t& localVars_26f6, const Packets_t& contexts_26f9, const Packets_t& functions_26fd, Packets_t& triggerPorts_26ef, Packets_t& argDeclBases_26f2, Packets_t& structs_26f5, Packets_t& localVars_26f8, Packets_t& contexts_26fb, Packets_t& triggerVars_26fc, Packets_t& functions_26ff);

protected:
	bool isInputUnique( const Udm::Object& triggerPort_270b, const Udm::Object& argDeclBase_2714, const Udm::Object& struct_271d, const Udm::Object& localVar_2726, const Udm::Object& context_272f, const Udm::Object& function_2738);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& triggerPorts_26ed, const Packets_t& argDeclBases_26f0, const Packets_t& structs_26f3, const Packets_t& localVars_26f6, const Packets_t& contexts_26f9, const Packets_t& functions_26fd);
	bool patternMatcher( const Udm::Object& triggerPort_2709, const Udm::Object& argDeclBase_2712, const Udm::Object& struct_271b, const Udm::Object& localVar_2724, const Udm::Object& context_272d, const Udm::Object& function_2736);
	void effector();
	void outputAppender( const CyberComposition::Simulink::TriggerPort& triggerPort_2748, const SFC::ArgDeclBase& argDeclBase_274a, const SFC::Struct& struct_274c, const SFC::LocalVar& localVar_274e, const SFC::Arg& context_2750, const SFC::LocalVar& triggerVar_2752, const SFC::Function& function_2754);

private:
	Packets_t* _triggerPort_2700;
	Packets_t* _argDeclBase_2701;
	Packets_t* _struct_2702;
	Packets_t* _localVar_2703;
	Packets_t* _context_2704;
	Packets_t* _triggerVar_2705;
	Packets_t* _function_2706;
	Packets_t _triggerPort_2707;
	Packets_t _argDeclBase_2710;
	Packets_t _struct_2719;
	Packets_t _localVar_2722;
	Packets_t _context_272b;
	Packets_t _function_2734;
	class Match
	{
	public:
		CyberComposition::Simulink::TriggerPort triggerPort_2741;
		SFC::ArgDeclBase argDeclBase_2742;
		SFC::Struct struct_2743;
		SFC::LocalVar localVar_2744;
		SFC::Arg context_2745;
		SFC::Function function_2746;
	};

	std::list< Match> _matches;
};

class StartAssignment_2756
{
public:
	void operator()( const Packets_t& triggerPorts_2757, const Packets_t& argDeclBases_275a, const Packets_t& structs_275e, const Packets_t& oldvals_2760, const Packets_t& contexts_2763, const Packets_t& triggerVars_2766, const Packets_t& functions_2768, Packets_t& triggerPorts_2759, Packets_t& argDeclBases_275c, Packets_t& localVars_275d, Packets_t& oldvals_2762, Packets_t& contexts_2765, Packets_t& unaryExprss_276a);

protected:
	bool isInputUnique( const Udm::Object& triggerPort_2775, const Udm::Object& argDeclBase_277e, const Udm::Object& struct_2787, const Udm::Object& oldval_2790, const Udm::Object& context_2799, const Udm::Object& triggerVar_27a2, const Udm::Object& function_27ab);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& triggerPorts_2757, const Packets_t& argDeclBases_275a, const Packets_t& structs_275e, const Packets_t& oldvals_2760, const Packets_t& contexts_2763, const Packets_t& triggerVars_2766, const Packets_t& functions_2768);
	bool patternMatcher( const Udm::Object& triggerPort_2773, const Udm::Object& argDeclBase_277c, const Udm::Object& struct_2785, const Udm::Object& oldval_278e, const Udm::Object& context_2797, const Udm::Object& triggerVar_27a0, const Udm::Object& function_27a9);
	void effector();
	void outputAppender( const CyberComposition::Simulink::TriggerPort& triggerPort_27c5, const SFC::ArgDeclBase& argDeclBase_27c7, const SFC::LocalVar& localVar_27c9, const SFC::LocalVar& oldval_27cb, const SFC::Arg& context_27cd, const SFC::UnaryExprs& unaryExprs_27cf);

private:
	Packets_t* _triggerPort_276b;
	Packets_t* _argDeclBase_276c;
	Packets_t* _localVar_276d;
	Packets_t* _oldval_276e;
	Packets_t* _context_276f;
	Packets_t* _unaryExprs_2770;
	Packets_t _triggerPort_2771;
	Packets_t _argDeclBase_277a;
	Packets_t _struct_2783;
	Packets_t _oldval_278c;
	Packets_t _context_2795;
	Packets_t _triggerVar_279e;
	Packets_t _function_27a7;
	class Match
	{
	public:
		CyberComposition::Simulink::TriggerPort triggerPort_27b7;
		SFC::ArgDeclBase argDeclBase_27b8;
		SFC::Struct struct_27b9;
		SFC::LocalVar oldval_27ba;
		SFC::Arg context_27bb;
		SFC::LocalVar triggerVar_27bc;
		SFC::Function function_27bd;
		SFC::LocalVar localVar_27be;
	};

	std::list< Match> _matches;
};

class SaveState_2813
{
public:
	void operator()( const Packets_t& systems_2814, const Packets_t& triggers_2816, const Packets_t& argdecls_2818, const Packets_t& dts_281a, const Packets_t& oldvals_281c, const Packets_t& contexts_281e, const Packets_t& functions_2820, Packets_t& systems_2822, Packets_t& triggers_2823, Packets_t& dts_2824, Packets_t& oldvals_2825, Packets_t& functions_2826);

protected:
	void callSaveMatrixState_28f3( const Packets_t& argdecls_288d, const Packets_t& dts_288f, const Packets_t& oldvals_2891, const Packets_t& contexts_2893, const Packets_t& functions_2895);
	void callSaveStructState_28f9( const Packets_t& argdecls_282d, const Packets_t& dts_282f, const Packets_t& oldvals_2831, const Packets_t& contexts_2833, const Packets_t& functions_2835);

private:
	Packets_t* _system_2827;
	Packets_t* _trigger_2828;
	Packets_t* _dt_2829;
	Packets_t* _oldval_282a;
	Packets_t* _function_282b;
};

class SaveStructState_282c
{
public:
	void operator()( const Packets_t& argdecls_282d, const Packets_t& dts_282f, const Packets_t& oldvals_2831, const Packets_t& contexts_2833, const Packets_t& functions_2835);

protected:
	void callSaveStructState_2886( const Packets_t& argDeclBases_2838, const Packets_t& structs_283a, const Packets_t& localVars_283c, const Packets_t& contexts_283e, const Packets_t& functions_2840);
};

class SaveStructState_2837
{
public:
	void operator()( const Packets_t& argDeclBases_2838, const Packets_t& structs_283a, const Packets_t& localVars_283c, const Packets_t& contexts_283e, const Packets_t& functions_2840);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_2846, const Udm::Object& struct_284f, const Udm::Object& localVar_2858, const Udm::Object& context_2861, const Udm::Object& function_286a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_2838, const Packets_t& structs_283a, const Packets_t& localVars_283c, const Packets_t& contexts_283e, const Packets_t& functions_2840);
	bool patternMatcher( const Udm::Object& argDeclBase_2844, const Udm::Object& struct_284d, const Udm::Object& localVar_2856, const Udm::Object& context_285f, const Udm::Object& function_2868);
	void effector();

private:
	Packets_t _argDeclBase_2842;
	Packets_t _struct_284b;
	Packets_t _localVar_2854;
	Packets_t _context_285d;
	Packets_t _function_2866;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_2876;
		SFC::Struct struct_2877;
		SFC::LocalVar localVar_2878;
		SFC::Arg context_2879;
		SFC::Function function_287a;
		SFC::LocalVar member_287b;
	};

	std::list< Match> _matches;
};

class SaveMatrixState_288c
{
public:
	void operator()( const Packets_t& argdecls_288d, const Packets_t& dts_288f, const Packets_t& oldvals_2891, const Packets_t& contexts_2893, const Packets_t& functions_2895, Packets_t& argdecls_2897, Packets_t& dts_2898, Packets_t& oldvals_2899, Packets_t& contexts_289a, Packets_t& functions_289b);

protected:
	void callSaveMatrixState_28ed( const Packets_t& argDeclBases_28a2, const Packets_t& dTs_28a4, const Packets_t& localVars_28a6, const Packets_t& args_28a8, const Packets_t& functions_28aa);

private:
	Packets_t* _argdecl_289c;
	Packets_t* _dt_289d;
	Packets_t* _oldval_289e;
	Packets_t* _context_289f;
	Packets_t* _function_28a0;
};

class SaveMatrixState_28a1
{
public:
	void operator()( const Packets_t& argDeclBases_28a2, const Packets_t& dTs_28a4, const Packets_t& localVars_28a6, const Packets_t& args_28a8, const Packets_t& functions_28aa);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_28b0, const Udm::Object& dT_28b9, const Udm::Object& localVar_28c2, const Udm::Object& arg_28cb, const Udm::Object& function_28d4);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg, SFC::ArgDeclBase& ArgDeclBase, SFC::DT& DT, SFC::Function& Function, SFC::LocalVar& LocalVar);
	void processInputPackets( const Packets_t& argDeclBases_28a2, const Packets_t& dTs_28a4, const Packets_t& localVars_28a6, const Packets_t& args_28a8, const Packets_t& functions_28aa);
	bool patternMatcher( const Udm::Object& argDeclBase_28ae, const Udm::Object& dT_28b7, const Udm::Object& localVar_28c0, const Udm::Object& arg_28c9, const Udm::Object& function_28d2);
	void effector();

private:
	Packets_t _argDeclBase_28ac;
	Packets_t _dT_28b5;
	Packets_t _localVar_28be;
	Packets_t _arg_28c7;
	Packets_t _function_28d0;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_28dd;
		SFC::DT dT_28de;
		SFC::LocalVar localVar_28df;
		SFC::Arg arg_28e0;
		SFC::Function function_28e1;
	};

	std::list< Match> _matches;
};

class AddStateVar_28ff
{
public:
	void operator()( const Packets_t& subsystems_2900, const Packets_t& triggerPorts_2903, const Packets_t& argDeclBases_2906, const Packets_t& dTs_290a, const Packets_t& functions_290e, Packets_t& subsystems_2902, Packets_t& triggerPorts_2905, Packets_t& argDeclBases_2908, Packets_t& localVars_2909, Packets_t& dTs_290c, Packets_t& args_290d, Packets_t& functions_2910);

protected:
	bool isInputUnique( const Udm::Object& subsystem_291c, const Udm::Object& triggerPort_2925, const Udm::Object& argDeclBase_292e, const Udm::Object& dT_2937, const Udm::Object& function_2940);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_2900, const Packets_t& triggerPorts_2903, const Packets_t& argDeclBases_2906, const Packets_t& dTs_290a, const Packets_t& functions_290e);
	bool patternMatcher( const Udm::Object& subsystem_291a, const Udm::Object& triggerPort_2923, const Udm::Object& argDeclBase_292c, const Udm::Object& dT_2935, const Udm::Object& function_293e);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_295c, const CyberComposition::Simulink::TriggerPort& triggerPort_295e, const SFC::ArgDeclBase& argDeclBase_2960, const SFC::LocalVar& localVar_2962, const SFC::DT& dT_2964, const SFC::Arg& arg_2966, const SFC::Function& function_2968);

private:
	Packets_t* _subsystem_2911;
	Packets_t* _triggerPort_2912;
	Packets_t* _argDeclBase_2913;
	Packets_t* _localVar_2914;
	Packets_t* _dT_2915;
	Packets_t* _arg_2916;
	Packets_t* _function_2917;
	Packets_t _subsystem_2918;
	Packets_t _triggerPort_2921;
	Packets_t _argDeclBase_292a;
	Packets_t _dT_2933;
	Packets_t _function_293c;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_2953;
		CyberComposition::Simulink::TriggerPort triggerPort_2954;
		SFC::ArgDeclBase argDeclBase_2955;
		SFC::DT dT_2956;
		SFC::Function function_2957;
		SFC::Arg arg_2958;
		SFC::Struct struct_2959;
		SFC::Class class_295a;
	};

	std::list< Match> _matches;
};

class MakeCondition_296a
{
public:
	void operator()( const Packets_t& systems_296b, const Packets_t& triggers_296d, const Packets_t& dts_296f, const Packets_t& oldvals_2971, const Packets_t& functions_2973, Packets_t& systems_2975, Packets_t& childSubsystems_2976, Packets_t& conditionals_2977);

protected:
	void callCreateConditional_2c71( const Packets_t& subsystems_2c12, const Packets_t& triggerPorts_2c15, const Packets_t& dTs_2c18, const Packets_t& localVars_2c1b, const Packets_t& functions_2c1f);
	void callSetMatrixCondition_2c77( const Packets_t& dts_2bd4, const Packets_t& triggervars_2bd6, const Packets_t& conditions_2bd8);
	void callSetStructCondition_2c7b( const Packets_t& dts_297c, const Packets_t& oldvals_297e, const Packets_t& conditions_2980);

private:
	Packets_t* _system_2978;
	Packets_t* _childSubsystem_2979;
	Packets_t* _conditional_297a;
};

class SetStructCondition_297b
{
public:
	void operator()( const Packets_t& dts_297c, const Packets_t& oldvals_297e, const Packets_t& conditions_2980);

protected:
	void callStartCondition_2bc6( const Packets_t& structs_2983, const Packets_t& triggerVars_2986, const Packets_t& userCodes_2989);
	void callgetStructMembers_2bca( const Packets_t& structs_2b8a, const Packets_t& triggerVars_2b8e, const Packets_t& unaryExprss_2b91);
	void callPlaceMember_2bce( const Packets_t& structs_29b9, const Packets_t& members_29bb, const Packets_t& trigvars_29bd, const Packets_t& condexprs_29bf);
};

class StartCondition_2982
{
public:
	void operator()( const Packets_t& structs_2983, const Packets_t& triggerVars_2986, const Packets_t& userCodes_2989, Packets_t& structs_2985, Packets_t& triggerVars_2988, Packets_t& unaryExprss_298b);

protected:
	bool isInputUnique( const Udm::Object& struct_2993, const Udm::Object& triggerVar_299c, const Udm::Object& userCode_29a5);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& structs_2983, const Packets_t& triggerVars_2986, const Packets_t& userCodes_2989);
	bool patternMatcher( const Udm::Object& struct_2991, const Udm::Object& triggerVar_299a, const Udm::Object& userCode_29a3);
	void effector();
	void outputAppender( const SFC::Struct& struct_29b2, const SFC::LocalVar& triggerVar_29b4, const SFC::UnaryExprs& unaryExprs_29b6);

private:
	Packets_t* _struct_298c;
	Packets_t* _triggerVar_298d;
	Packets_t* _unaryExprs_298e;
	Packets_t _struct_298f;
	Packets_t _triggerVar_2998;
	Packets_t _userCode_29a1;
	class Match
	{
	public:
		SFC::Struct struct_29ae;
		SFC::LocalVar triggerVar_29af;
		SFC::UserCode userCode_29b0;
	};

	std::list< Match> _matches;
};

class PlaceMember_29b8
{
public:
	void operator()( const Packets_t& structs_29b9, const Packets_t& members_29bb, const Packets_t& trigvars_29bd, const Packets_t& condexprs_29bf);

protected:
	void executeOne( const Packets_t& structs_29b9, const Packets_t& members_29bb, const Packets_t& trigvars_29bd, const Packets_t& condexprs_29bf);
	bool isInputUnique( const Udm::Object& struct_29c3, const Udm::Object& member_29ca, const Udm::Object& trigvar_29d1, const Udm::Object& condexpr_29d8);
	void callTriggerTest_2b7a( const Packets_t& structs_2a6b, const Packets_t& members_2a6d, const Packets_t& trigvars_2a6f, const Packets_t& condexprs_2a71);
	void callAddTrigger_2b7f( const Packets_t& structs_29de, const Packets_t& members_29e0, const Packets_t& trigvars_29e2, const Packets_t& condexprs_29e4);
	void callTraverseBinary_2b84( const Packets_t& structs_2b2e, const Packets_t& members_2b31, const Packets_t& triggerVars_2b34, const Packets_t& unaryExprss_2b37);

private:
	Packets_t _struct_29c1;
	Packets_t _member_29c8;
	Packets_t _trigvar_29cf;
	Packets_t _condexpr_29d6;
};

class AddTrigger_29dd
{
public:
	void operator()( const Packets_t& structs_29de, const Packets_t& members_29e0, const Packets_t& trigvars_29e2, const Packets_t& condexprs_29e4);

protected:
	void callAddLastTrigger_2a60( const Packets_t& structs_29e7, const Packets_t& members_29e9, const Packets_t& triggerVars_29eb, const Packets_t& unaryExprss_29ed);
	void callAddNextTrigger_2a65( const Packets_t& structs_2a23, const Packets_t& members_2a25, const Packets_t& triggerVars_2a27, const Packets_t& unaryExprss_2a29);
};

class AddLastTrigger_29e6
{
public:
	void operator()( const Packets_t& structs_29e7, const Packets_t& members_29e9, const Packets_t& triggerVars_29eb, const Packets_t& unaryExprss_29ed);

protected:
	bool isInputUnique( const Udm::Object& struct_29f3, const Udm::Object& member_29fc, const Udm::Object& triggerVar_2a05, const Udm::Object& unaryExprs_2a0e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::LocalVar& Member, SFC::Struct& Struct, SFC::LocalVar& TriggerVar, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& structs_29e7, const Packets_t& members_29e9, const Packets_t& triggerVars_29eb, const Packets_t& unaryExprss_29ed);
	bool patternMatcher( const Udm::Object& struct_29f1, const Udm::Object& member_29fa, const Udm::Object& triggerVar_2a03, const Udm::Object& unaryExprs_2a0c);
	void effector();

private:
	Packets_t _struct_29ef;
	Packets_t _member_29f8;
	Packets_t _triggerVar_2a01;
	Packets_t _unaryExprs_2a0a;
	class Match
	{
	public:
		SFC::Struct struct_2a17;
		SFC::LocalVar member_2a18;
		SFC::LocalVar triggerVar_2a19;
		SFC::UnaryExprs unaryExprs_2a1a;
	};

	std::list< Match> _matches;
};

class AddNextTrigger_2a22
{
public:
	void operator()( const Packets_t& structs_2a23, const Packets_t& members_2a25, const Packets_t& triggerVars_2a27, const Packets_t& unaryExprss_2a29);

protected:
	bool isInputUnique( const Udm::Object& struct_2a2f, const Udm::Object& member_2a38, const Udm::Object& triggerVar_2a41, const Udm::Object& unaryExprs_2a4a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::LocalVar& Member, SFC::Struct& Struct, SFC::LocalVar& TriggerVar, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& structs_2a23, const Packets_t& members_2a25, const Packets_t& triggerVars_2a27, const Packets_t& unaryExprss_2a29);
	bool patternMatcher( const Udm::Object& struct_2a2d, const Udm::Object& member_2a36, const Udm::Object& triggerVar_2a3f, const Udm::Object& unaryExprs_2a48);
	void effector();

private:
	Packets_t _struct_2a2b;
	Packets_t _member_2a34;
	Packets_t _triggerVar_2a3d;
	Packets_t _unaryExprs_2a46;
	class Match
	{
	public:
		SFC::Struct struct_2a53;
		SFC::LocalVar member_2a54;
		SFC::LocalVar triggerVar_2a55;
		SFC::UnaryExprs unaryExprs_2a56;
	};

	std::list< Match> _matches;
};

class TriggerTest_2a6a
{
public:
	void operator()( const Packets_t& structs_2a6b, const Packets_t& members_2a6d, const Packets_t& trigvars_2a6f, const Packets_t& condexprs_2a71, Packets_t& structs_2a73, Packets_t& members_2a74, Packets_t& trigvars_2a75, Packets_t& condexprs_2a76, Packets_t& structs_2a77, Packets_t& members_2a78, Packets_t& trigvars_2a79, Packets_t& condexprs_2a7a);

protected:
	void executeOne( const Packets_t& structs_2a6b, const Packets_t& members_2a6d, const Packets_t& trigvars_2a6f, const Packets_t& condexprs_2a71);
	bool isInputUnique( const Udm::Object& struct_2a85, const Udm::Object& member_2a8c, const Udm::Object& trigvar_2a93, const Udm::Object& condexpr_2a9a);

private:
	Packets_t* _struct_2a7b;
	Packets_t* _member_2a7c;
	Packets_t* _trigvar_2a7d;
	Packets_t* _condexpr_2a7e;
	Packets_t* _struct_2a7f;
	Packets_t* _member_2a80;
	Packets_t* _trigvar_2a81;
	Packets_t* _condexpr_2a82;
	Packets_t _struct_2a83;
	Packets_t _member_2a8a;
	Packets_t _trigvar_2a91;
	Packets_t _condexpr_2a98;
};

class HasBinaryExprs_2a9f
{
public:
	bool operator()( const Packets_t& structs_2aa0, const Packets_t& members_2aa3, const Packets_t& triggerVars_2aa6, const Packets_t& unaryExprss_2aa9, Packets_t& structs_2aa2, Packets_t& members_2aa5, Packets_t& triggerVars_2aa8, Packets_t& unaryExprss_2aab);

protected:
	bool isInputUnique( const Udm::Object& struct_2ab4, const Udm::Object& member_2abd, const Udm::Object& triggerVar_2ac6, const Udm::Object& unaryExprs_2acf);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& structs_2aa0, const Packets_t& members_2aa3, const Packets_t& triggerVars_2aa6, const Packets_t& unaryExprss_2aa9);
	bool patternMatcher( const Udm::Object& struct_2ab2, const Udm::Object& member_2abb, const Udm::Object& triggerVar_2ac4, const Udm::Object& unaryExprs_2acd);
	void outputAppender( const SFC::Struct& struct_2ae0, const SFC::LocalVar& member_2ae2, const SFC::LocalVar& triggerVar_2ae4, const SFC::UnaryExprs& unaryExprs_2ae6);

private:
	Packets_t* _struct_2aac;
	Packets_t* _member_2aad;
	Packets_t* _triggerVar_2aae;
	Packets_t* _unaryExprs_2aaf;
	Packets_t _struct_2ab0;
	Packets_t _member_2ab9;
	Packets_t _triggerVar_2ac2;
	Packets_t _unaryExprs_2acb;
	class Match
	{
	public:
		SFC::Struct struct_2adb;
		SFC::LocalVar member_2adc;
		SFC::LocalVar triggerVar_2add;
		SFC::UnaryExprs unaryExprs_2ade;
		SFC::BinaryExprs binaryExprs_2adf;
	};

	std::list< Match> _matches;
};

class Otherwise_2ae8
{
public:
	bool operator()( const Packets_t& structs_2ae9, const Packets_t& members_2aec, const Packets_t& triggerVars_2aef, const Packets_t& unaryExprss_2af2, Packets_t& structs_2aeb, Packets_t& members_2aee, Packets_t& triggerVars_2af1, Packets_t& unaryExprss_2af4);

protected:
	bool isInputUnique( const Udm::Object& struct_2afd, const Udm::Object& member_2b06, const Udm::Object& triggerVar_2b0f, const Udm::Object& unaryExprs_2b18);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& structs_2ae9, const Packets_t& members_2aec, const Packets_t& triggerVars_2aef, const Packets_t& unaryExprss_2af2);
	bool patternMatcher( const Udm::Object& struct_2afb, const Udm::Object& member_2b04, const Udm::Object& triggerVar_2b0d, const Udm::Object& unaryExprs_2b16);
	void outputAppender( const SFC::Struct& struct_2b25, const SFC::LocalVar& member_2b27, const SFC::LocalVar& triggerVar_2b29, const SFC::UnaryExprs& unaryExprs_2b2b);

private:
	Packets_t* _struct_2af5;
	Packets_t* _member_2af6;
	Packets_t* _triggerVar_2af7;
	Packets_t* _unaryExprs_2af8;
	Packets_t _struct_2af9;
	Packets_t _member_2b02;
	Packets_t _triggerVar_2b0b;
	Packets_t _unaryExprs_2b14;
	class Match
	{
	public:
		SFC::Struct struct_2b21;
		SFC::LocalVar member_2b22;
		SFC::LocalVar triggerVar_2b23;
		SFC::UnaryExprs unaryExprs_2b24;
	};

	std::list< Match> _matches;
};

class TraverseBinary_2b2d
{
public:
	void operator()( const Packets_t& structs_2b2e, const Packets_t& members_2b31, const Packets_t& triggerVars_2b34, const Packets_t& unaryExprss_2b37, Packets_t& structs_2b30, Packets_t& members_2b33, Packets_t& triggerVars_2b36, Packets_t& unaryExprss_2b39);

protected:
	bool isInputUnique( const Udm::Object& struct_2b42, const Udm::Object& member_2b4b, const Udm::Object& triggerVar_2b54, const Udm::Object& unaryExprs_2b5d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& structs_2b2e, const Packets_t& members_2b31, const Packets_t& triggerVars_2b34, const Packets_t& unaryExprss_2b37);
	bool patternMatcher( const Udm::Object& struct_2b40, const Udm::Object& member_2b49, const Udm::Object& triggerVar_2b52, const Udm::Object& unaryExprs_2b5b);
	void effector();
	void outputAppender( const SFC::Struct& struct_2b72, const SFC::LocalVar& member_2b74, const SFC::LocalVar& triggerVar_2b76, const SFC::UnaryExprs& unaryExprs_2b78);

private:
	Packets_t* _struct_2b3a;
	Packets_t* _member_2b3b;
	Packets_t* _triggerVar_2b3c;
	Packets_t* _unaryExprs_2b3d;
	Packets_t _struct_2b3e;
	Packets_t _member_2b47;
	Packets_t _triggerVar_2b50;
	Packets_t _unaryExprs_2b59;
	class Match
	{
	public:
		SFC::Struct struct_2b6c;
		SFC::LocalVar member_2b6d;
		SFC::LocalVar triggerVar_2b6e;
		SFC::UnaryExprs unaryExprs_2b6f;
		SFC::BinaryExprs binaryExprs_2b70;
		SFC::UnaryExprs unaryExprs_2b71;
	};

	std::list< Match> _matches;
};

class GetStructMembers_2b89
{
public:
	void operator()( const Packets_t& structs_2b8a, const Packets_t& triggerVars_2b8e, const Packets_t& unaryExprss_2b91, Packets_t& structs_2b8c, Packets_t& members_2b8d, Packets_t& triggerVars_2b90, Packets_t& unaryExprss_2b93);

protected:
	bool isInputUnique( const Udm::Object& struct_2b9c, const Udm::Object& triggerVar_2ba5, const Udm::Object& unaryExprs_2bae);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& structs_2b8a, const Packets_t& triggerVars_2b8e, const Packets_t& unaryExprss_2b91);
	bool patternMatcher( const Udm::Object& struct_2b9a, const Udm::Object& triggerVar_2ba3, const Udm::Object& unaryExprs_2bac);
	void effector();
	void outputAppender( const SFC::Struct& struct_2bbe, const SFC::LocalVar& member_2bc0, const SFC::LocalVar& triggerVar_2bc2, const SFC::UnaryExprs& unaryExprs_2bc4);
	void sortOutputs();

private:
	Packets_t* _struct_2b94;
	Packets_t* _member_2b95;
	Packets_t* _triggerVar_2b96;
	Packets_t* _unaryExprs_2b97;
	Packets_t _struct_2b98;
	Packets_t _triggerVar_2ba1;
	Packets_t _unaryExprs_2baa;
	class Match
	{
	public:
		SFC::Struct struct_2bba;
		SFC::LocalVar triggerVar_2bbb;
		SFC::UnaryExprs unaryExprs_2bbc;
		SFC::LocalVar member_2bbd;
	};

	std::list< Match> _matches;
};

class SetMatrixCondition_2bd3
{
public:
	void operator()( const Packets_t& dts_2bd4, const Packets_t& triggervars_2bd6, const Packets_t& conditions_2bd8, Packets_t& dts_2bda, Packets_t& triggervars_2bdb, Packets_t& conditions_2bdc);

protected:
	void callMatrixCondition_2c0d( const Packets_t& dTs_2be1, const Packets_t& triggerVars_2be3, const Packets_t& conditions_2be5);

private:
	Packets_t* _dt_2bdd;
	Packets_t* _triggervar_2bde;
	Packets_t* _condition_2bdf;
};

class MatrixCondition_2be0
{
public:
	void operator()( const Packets_t& dTs_2be1, const Packets_t& triggerVars_2be3, const Packets_t& conditions_2be5);

protected:
	bool isInputUnique( const Udm::Object& dT_2beb, const Udm::Object& triggerVar_2bf4, const Udm::Object& condition_2bfd);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::UserCode& Condition, SFC::DT& DT, SFC::LocalVar& TriggerVar);
	void processInputPackets( const Packets_t& dTs_2be1, const Packets_t& triggerVars_2be3, const Packets_t& conditions_2be5);
	bool patternMatcher( const Udm::Object& dT_2be9, const Udm::Object& triggerVar_2bf2, const Udm::Object& condition_2bfb);
	void effector();

private:
	Packets_t _dT_2be7;
	Packets_t _triggerVar_2bf0;
	Packets_t _condition_2bf9;
	class Match
	{
	public:
		SFC::DT dT_2c06;
		SFC::LocalVar triggerVar_2c07;
		SFC::UserCode condition_2c08;
	};

	std::list< Match> _matches;
};

class CreateConditional_2c11
{
public:
	void operator()( const Packets_t& subsystems_2c12, const Packets_t& triggerPorts_2c15, const Packets_t& dTs_2c18, const Packets_t& localVars_2c1b, const Packets_t& functions_2c1f, Packets_t& subsystems_2c14, Packets_t& charts_2c17, Packets_t& dTs_2c1a, Packets_t& triggerVars_2c1d, Packets_t& userCodes_2c1e, Packets_t& conditionalBlocks_2c21);

protected:
	bool isInputUnique( const Udm::Object& subsystem_2c2c, const Udm::Object& triggerPort_2c35, const Udm::Object& dT_2c3e, const Udm::Object& localVar_2c47, const Udm::Object& function_2c50);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_2c12, const Packets_t& triggerPorts_2c15, const Packets_t& dTs_2c18, const Packets_t& localVars_2c1b, const Packets_t& functions_2c1f);
	bool patternMatcher( const Udm::Object& subsystem_2c2a, const Udm::Object& triggerPort_2c33, const Udm::Object& dT_2c3c, const Udm::Object& localVar_2c45, const Udm::Object& function_2c4e);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_2c65, const CyberComposition::Simulink::Subsystem& chart_2c67, const SFC::DT& dT_2c69, const SFC::LocalVar& triggerVar_2c6b, const SFC::UserCode& userCode_2c6d, const SFC::ConditionalBlock& conditionalBlock_2c6f);

private:
	Packets_t* _subsystem_2c22;
	Packets_t* _chart_2c23;
	Packets_t* _dT_2c24;
	Packets_t* _triggerVar_2c25;
	Packets_t* _userCode_2c26;
	Packets_t* _conditionalBlock_2c27;
	Packets_t _subsystem_2c28;
	Packets_t _triggerPort_2c31;
	Packets_t _dT_2c3a;
	Packets_t _localVar_2c43;
	Packets_t _function_2c4c;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_2c5c;
		CyberComposition::Simulink::TriggerPort triggerPort_2c5d;
		SFC::DT dT_2c5e;
		SFC::LocalVar localVar_2c5f;
		SFC::Function function_2c60;
		CyberComposition::Simulink::Subsystem chart_2c61;
		SFC::LocalVar triggerVar_2c62;
	};

	std::list< Match> _matches;
};

class CreateAction_2ca9
{
public:
	void operator()( const Packets_t& subsystems_2caa, const Packets_t& blocks_2cad, const Packets_t& functions_2cb0, Packets_t& subsystems_2cac, Packets_t& blocks_2caf, Packets_t& conditionalBlocks_2cb2);

protected:
	bool isInputUnique( const Udm::Object& subsystem_2cba, const Udm::Object& block_2cc3, const Udm::Object& function_2ccc);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_2caa, const Packets_t& blocks_2cad, const Packets_t& functions_2cb0);
	bool patternMatcher( const Udm::Object& subsystem_2cb8, const Udm::Object& block_2cc1, const Udm::Object& function_2cca);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_2ce3, const CyberComposition::Simulink::Block& block_2ce5, const SFC::ConditionalBlock& conditionalBlock_2ce7);

private:
	Packets_t* _subsystem_2cb3;
	Packets_t* _block_2cb4;
	Packets_t* _conditionalBlock_2cb5;
	Packets_t _subsystem_2cb6;
	Packets_t _block_2cbf;
	Packets_t _function_2cc8;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_2cd9;
		CyberComposition::Simulink::Block block_2cda;
		SFC::Function function_2cdb;
		CyberComposition::Simulink::EnablePort actionPort_2cdc;
		SFC::ArgDeclBase argDeclBase_2cdd;
	};

	std::list< Match> _matches;
};

class MakeArgDeclRef_2cf5
{
public:
	void operator()( const Packets_t& systems_2cf6, const Packets_t& argPorts_2cf8, const Packets_t& argVals_2cfa);

protected:
	void callCheckSigSrc_2de6( const Packets_t& systems_2d29, const Packets_t& argPorts_2d2b, const Packets_t& argVals_2d2d);
	void callInpArgValTrigger_2dea( const Packets_t& subsystems_2cfd, const Packets_t& triggerPorts_2cff, const Packets_t& argVals_2d01);
	void callInpArgVal_2dee( const Packets_t& subsystems_2dbb, const Packets_t& dstPorts_2dbd, const Packets_t& argVals_2dbf);
};

class InpArgValTrigger_2cfc
{
public:
	void operator()( const Packets_t& subsystems_2cfd, const Packets_t& triggerPorts_2cff, const Packets_t& argVals_2d01);

protected:
	bool isInputUnique( const Udm::Object& subsystem_2d07, const Udm::Object& triggerPort_2d10, const Udm::Object& argVal_2d19);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_2cfd, const Packets_t& triggerPorts_2cff, const Packets_t& argVals_2d01);
	bool patternMatcher( const Udm::Object& subsystem_2d05, const Udm::Object& triggerPort_2d0e, const Udm::Object& argVal_2d17);
	void effector();

private:
	Packets_t _subsystem_2d03;
	Packets_t _triggerPort_2d0c;
	Packets_t _argVal_2d15;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_2d23;
		CyberComposition::Simulink::TriggerPort triggerPort_2d24;
		SFC::ArgVal argVal_2d25;
		SFC::LocalVar localVar_2d26;
	};

	std::list< Match> _matches;
};

class CheckSigSrc_2d28
{
public:
	void operator()( const Packets_t& systems_2d29, const Packets_t& argPorts_2d2b, const Packets_t& argVals_2d2d, Packets_t& systems_2d2f, Packets_t& argPorts_2d30, Packets_t& argVals_2d31, Packets_t& systems_2d32, Packets_t& argPorts_2d33, Packets_t& argVals_2d34);

protected:
	void executeOne( const Packets_t& systems_2d29, const Packets_t& argPorts_2d2b, const Packets_t& argVals_2d2d);
	bool isInputUnique( const Udm::Object& system_2d3d, const Udm::Object& argPort_2d44, const Udm::Object& argVal_2d4b);

private:
	Packets_t* _system_2d35;
	Packets_t* _argPort_2d36;
	Packets_t* _argVal_2d37;
	Packets_t* _system_2d38;
	Packets_t* _argPort_2d39;
	Packets_t* _argVal_2d3a;
	Packets_t _system_2d3b;
	Packets_t _argPort_2d42;
	Packets_t _argVal_2d49;
};

class IsTrigger_2d50
{
public:
	bool operator()( const Packets_t& subsystems_2d51, const Packets_t& triggerPorts_2d54, const Packets_t& argVals_2d57, Packets_t& subsystems_2d53, Packets_t& triggerPorts_2d56, Packets_t& argVals_2d59);

protected:
	bool isInputUnique( const Udm::Object& subsystem_2d61, const Udm::Object& triggerPort_2d6a, const Udm::Object& argVal_2d73);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_2d51, const Packets_t& triggerPorts_2d54, const Packets_t& argVals_2d57);
	bool patternMatcher( const Udm::Object& subsystem_2d5f, const Udm::Object& triggerPort_2d68, const Udm::Object& argVal_2d71);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_2d7f, const CyberComposition::Simulink::TriggerPort& triggerPort_2d81, const SFC::ArgVal& argVal_2d83);

private:
	Packets_t* _subsystem_2d5a;
	Packets_t* _triggerPort_2d5b;
	Packets_t* _argVal_2d5c;
	Packets_t _subsystem_2d5d;
	Packets_t _triggerPort_2d66;
	Packets_t _argVal_2d6f;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_2d7c;
		CyberComposition::Simulink::TriggerPort triggerPort_2d7d;
		SFC::ArgVal argVal_2d7e;
	};

	std::list< Match> _matches;
};

class Otherwise_2d85
{
public:
	bool operator()( const Packets_t& subsystems_2d86, const Packets_t& dstPorts_2d89, const Packets_t& argVals_2d8c, Packets_t& subsystems_2d88, Packets_t& dstPorts_2d8b, Packets_t& argVals_2d8e);

protected:
	bool isInputUnique( const Udm::Object& subsystem_2d96, const Udm::Object& dstPort_2d9f, const Udm::Object& argVal_2da8);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_2d86, const Packets_t& dstPorts_2d89, const Packets_t& argVals_2d8c);
	bool patternMatcher( const Udm::Object& subsystem_2d94, const Udm::Object& dstPort_2d9d, const Udm::Object& argVal_2da6);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_2db4, const CyberComposition::Simulink::SF_Port& dstPort_2db6, const SFC::ArgVal& argVal_2db8);

private:
	Packets_t* _subsystem_2d8f;
	Packets_t* _dstPort_2d90;
	Packets_t* _argVal_2d91;
	Packets_t _subsystem_2d92;
	Packets_t _dstPort_2d9b;
	Packets_t _argVal_2da4;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_2db1;
		CyberComposition::Simulink::SF_Port dstPort_2db2;
		SFC::ArgVal argVal_2db3;
	};

	std::list< Match> _matches;
};

class InpArgVal_2dba
{
public:
	void operator()( const Packets_t& subsystems_2dbb, const Packets_t& dstPorts_2dbd, const Packets_t& argVals_2dbf);

protected:
	bool isInputUnique( const Udm::Object& subsystem_2dc5, const Udm::Object& dstPort_2dce, const Udm::Object& argVal_2dd7);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_2dbb, const Packets_t& dstPorts_2dbd, const Packets_t& argVals_2dbf);
	bool patternMatcher( const Udm::Object& subsystem_2dc3, const Udm::Object& dstPort_2dcc, const Udm::Object& argVal_2dd5);
	void effector();

private:
	Packets_t _subsystem_2dc1;
	Packets_t _dstPort_2dca;
	Packets_t _argVal_2dd3;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_2de1;
		CyberComposition::Simulink::SF_Port dstPort_2de2;
		SFC::ArgVal argVal_2de3;
		SFC::ArgDeclBase argDeclBase_2de4;
	};

	std::list< Match> _matches;
};

class PrimitiveChildBlock_2e02
{
public:
	void operator()( const Packets_t& childPrimitives_2e03, const Packets_t& systemFunctions_2e05);

protected:
	void callCreateCGCall_3053( const Packets_t& childPrimitives_2e08, const Packets_t& systemFunctions_2e0a);
	void callCallCG_3056( const Packets_t& primitives_2f32, const Packets_t& compoundStatements_2f35);
	void callSetupIfVar_3059( const Packets_t& childPrimitives_2f53, const Packets_t& systemFunctions_2f55);
};

class CreateCGCall_2e07
{
public:
	void operator()( const Packets_t& childPrimitives_2e08, const Packets_t& systemFunctions_2e0a, Packets_t& childPrimitives_2e0c, Packets_t& systemFunctions_2e0d);

protected:
	void callInitEBCall_2f25( const Packets_t& primitives_2e11, const Packets_t& parents_2e14);
	void callGetSortedOutPorts_2f28( const Packets_t& functions_2e30);
	void callMapParams_2f2a( const Packets_t& primitives_2e4d);
	void callGetInputs_2f2c( const Packets_t& childPrimitives_2e81);
	void callListOutPorts_2f2e( const Packets_t& primitives_2e62, const Packets_t& outputPorts_2e64);

private:
	Packets_t* _childPrimitive_2e0e;
	Packets_t* _systemFunction_2e0f;
};

class InitEBCall_2e10
{
public:
	void operator()( const Packets_t& primitives_2e11, const Packets_t& parents_2e14, Packets_t& primitives_2e13);

protected:
	bool isInputUnique( const Udm::Object& primitive_2e1b, const Udm::Object& parent_2e24);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& primitives_2e11, const Packets_t& parents_2e14);
	bool patternMatcher( const Udm::Object& primitive_2e19, const Udm::Object& parent_2e22);
	void effector();
	void forwardInputs();

private:
	Packets_t* _primitive_2e16;
	Packets_t _primitive_2e17;
	Packets_t _parent_2e20;
	class Match
	{
	public:
		CyberComposition::Simulink::Primitive primitive_2e2d;
		SFC::CompoundStatement parent_2e2e;
	};

	std::list< Match> _matches;
};

class GetSortedOutPorts_2e2f
{
public:
	void operator()( const Packets_t& functions_2e30, Packets_t& functions_2e32, Packets_t& outputPorts_2e33);

protected:
	bool isInputUnique( const Udm::Object& function_2e3a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& functions_2e30);
	bool patternMatcher( const Udm::Object& function_2e38);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Primitive& function_2e48, const CyberComposition::Simulink::OutputPort& outputPort_2e4a);
	void sortOutputs();

private:
	Packets_t* _function_2e34;
	Packets_t* _outputPort_2e35;
	Packets_t _function_2e36;
	class Match
	{
	public:
		CyberComposition::Simulink::Primitive function_2e46;
		CyberComposition::Simulink::OutputPort outputPort_2e47;
	};

	std::list< Match> _matches;
};

class MapParams_2e4c
{
public:
	void operator()( const Packets_t& primitives_2e4d);

protected:
	bool isInputUnique( const Udm::Object& primitive_2e53);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& primitives_2e4d);
	bool patternMatcher( const Udm::Object& primitive_2e51);
	void effector();

private:
	Packets_t _primitive_2e4f;
	class Match
	{
	public:
		CyberComposition::Simulink::Primitive primitive_2e5f;
		CyberComposition::Simulink::SF_Parameter parameter_2e60;
	};

	std::list< Match> _matches;
};

class ListOutPorts_2e61
{
public:
	void operator()( const Packets_t& primitives_2e62, const Packets_t& outputPorts_2e64);

protected:
	bool isInputUnique( const Udm::Object& primitive_2e6a, const Udm::Object& outputPort_2e73);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& primitives_2e62, const Packets_t& outputPorts_2e64);
	bool patternMatcher( const Udm::Object& primitive_2e68, const Udm::Object& outputPort_2e71);
	void effector();

private:
	Packets_t _primitive_2e66;
	Packets_t _outputPort_2e6f;
	class Match
	{
	public:
		CyberComposition::Simulink::Primitive primitive_2e7d;
		CyberComposition::Simulink::OutputPort outputPort_2e7e;
		SFC::ArgDeclBase argDeclBase_2e7f;
	};

	std::list< Match> _matches;
};

class GetInputs_2e80
{
public:
	void operator()( const Packets_t& childPrimitives_2e81);

protected:
	void callProcessMerge_2f21( const Packets_t& childPrimitives_2e84);
	void callGetInputPorts_2f23( const Packets_t& childPrimitives_2ede);
};

class ProcessMerge_2e83
{
public:
	void operator()( const Packets_t& childPrimitives_2e84, Packets_t& childPrimitives_2e86);

protected:
	void callGetMergeBlocks_2ed9( const Packets_t& childPrimitives_2e89);
	void callMergeAsSwitch_2edb( const Packets_t& primitives_2eab);

private:
	Packets_t* _childPrimitive_2e87;
};

class GetMergeBlocks_2e88
{
public:
	void operator()( const Packets_t& childPrimitives_2e89, Packets_t& childPrimitives_2e8b);

protected:
	void executeOne( const Packets_t& childPrimitives_2e89);
	bool isInputUnique( const Udm::Object& childPrimitive_2e8f);

private:
	Packets_t* _childPrimitive_2e8c;
	Packets_t _childPrimitive_2e8d;
};

class MergeBlocksOnly_2e94
{
public:
	bool operator()( const Packets_t& primitives_2e95, Packets_t& primitives_2e97);

protected:
	bool isInputUnique( const Udm::Object& primitive_2e9d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::Primitive& Primitive);
	void processInputPackets( const Packets_t& primitives_2e95);
	bool patternMatcher( const Udm::Object& primitive_2e9b);
	void outputAppender( const CyberComposition::Simulink::Primitive& primitive_2ea8);

private:
	Packets_t* _primitive_2e98;
	Packets_t _primitive_2e99;
	class Match
	{
	public:
		CyberComposition::Simulink::Primitive primitive_2ea6;
	};

	std::list< Match> _matches;
};

class MergeAsSwitch_2eaa
{
public:
	void operator()( const Packets_t& primitives_2eab);

protected:
	bool isInputUnique( const Udm::Object& primitive_2eb1);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& primitives_2eab);
	bool patternMatcher( const Udm::Object& primitive_2eaf);
	void effector();

private:
	Packets_t _primitive_2ead;
	class Match
	{
	public:
		CyberComposition::Simulink::Primitive primitive_2ecf;
		CyberComposition::Simulink::InputPort inputPort_2ed0;
		SFC::LocalVar localVar_2ed1;
		CyberComposition::Simulink::OutputPort actionOutput_2ed2;
		CyberComposition::Simulink::Line line1_2ed3;
		CyberComposition::Simulink::Line line2_2ed4;
		CyberComposition::Simulink::Subsystem ifAction_2ed5;
		CyberComposition::Simulink::Primitive ifOrSwitchCaseBlock_2ed6;
		CyberComposition::Simulink::OutputPort ifOutput_2ed7;
		CyberComposition::Simulink::EnablePort actionPort_2ed8;
	};

	std::list< Match> _matches;
};

class GetInputPorts_2edd
{
public:
	void operator()( const Packets_t& childPrimitives_2ede);

protected:
	void callGetSortedInPorts_2f1c( const Packets_t& functions_2f00);
	void callListInPorts_2f1e( const Packets_t& primitives_2ee1, const Packets_t& inputPorts_2ee3);
};

class ListInPorts_2ee0
{
public:
	void operator()( const Packets_t& primitives_2ee1, const Packets_t& inputPorts_2ee3);

protected:
	bool isInputUnique( const Udm::Object& primitive_2ee9, const Udm::Object& inputPort_2ef2);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& primitives_2ee1, const Packets_t& inputPorts_2ee3);
	bool patternMatcher( const Udm::Object& primitive_2ee7, const Udm::Object& inputPort_2ef0);
	void effector();

private:
	Packets_t _primitive_2ee5;
	Packets_t _inputPort_2eee;
	class Match
	{
	public:
		CyberComposition::Simulink::Primitive primitive_2efc;
		CyberComposition::Simulink::SF_Port inputPort_2efd;
		SFC::ArgDeclBase argDeclBase_2efe;
	};

	std::list< Match> _matches;
};

class GetSortedInPorts_2eff
{
public:
	void operator()( const Packets_t& functions_2f00, Packets_t& functions_2f02, Packets_t& inputPorts_2f03);

protected:
	bool isInputUnique( const Udm::Object& function_2f0a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& functions_2f00);
	bool patternMatcher( const Udm::Object& function_2f08);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Primitive& function_2f18, const CyberComposition::Simulink::InputPort& inputPort_2f1a);
	void sortOutputs();

private:
	Packets_t* _function_2f04;
	Packets_t* _inputPort_2f05;
	Packets_t _function_2f06;
	class Match
	{
	public:
		CyberComposition::Simulink::Primitive function_2f16;
		CyberComposition::Simulink::InputPort inputPort_2f17;
	};

	std::list< Match> _matches;
};

class CallCG_2f31
{
public:
	void operator()( const Packets_t& primitives_2f32, const Packets_t& compoundStatements_2f35, Packets_t& primitives_2f34, Packets_t& compoundStatements_2f37);

protected:
	bool isInputUnique( const Udm::Object& primitive_2f3e, const Udm::Object& compoundStatement_2f47);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& primitives_2f32, const Packets_t& compoundStatements_2f35);
	bool patternMatcher( const Udm::Object& primitive_2f3c, const Udm::Object& compoundStatement_2f45);
	void effector();
	void forwardInputs();

private:
	Packets_t* _primitive_2f38;
	Packets_t* _compoundStatement_2f39;
	Packets_t _primitive_2f3a;
	Packets_t _compoundStatement_2f43;
	class Match
	{
	public:
		CyberComposition::Simulink::Primitive primitive_2f50;
		SFC::CompoundStatement compoundStatement_2f51;
	};

	std::list< Match> _matches;
};

class SetupIfVar_2f52
{
public:
	void operator()( const Packets_t& childPrimitives_2f53, const Packets_t& systemFunctions_2f55);

protected:
	void callGetIfAndSwitchCaseBlocks_3044( const Packets_t& childPrimitives_2fc4, const Packets_t& systemFunctions_2fc6);
	void callMakeIfVar_3047( const Packets_t& primitives_2f95, const Packets_t& compoundStatements_2f99);
	void callGetIfOutputPorts_304a( const Packets_t& primitives_2f58, const Packets_t& localVars_2f5c, const Packets_t& conditionalGroups_2f5f);
	void callAssignIfVar_304e( const Packets_t& primitives_3002, const Packets_t& outputPorts_3004, const Packets_t& ifVars_3006, const Packets_t& conditionalGroups_3008);
};

class GetIfOutputPorts_2f57
{
public:
	void operator()( const Packets_t& primitives_2f58, const Packets_t& localVars_2f5c, const Packets_t& conditionalGroups_2f5f, Packets_t& primitives_2f5a, Packets_t& outputPorts_2f5b, Packets_t& localVars_2f5e, Packets_t& conditionalGroups_2f61);

protected:
	bool isInputUnique( const Udm::Object& primitive_2f6a, const Udm::Object& localVar_2f73, const Udm::Object& conditionalGroup_2f7c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& primitives_2f58, const Packets_t& localVars_2f5c, const Packets_t& conditionalGroups_2f5f);
	bool patternMatcher( const Udm::Object& primitive_2f68, const Udm::Object& localVar_2f71, const Udm::Object& conditionalGroup_2f7a);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Primitive& primitive_2f8c, const CyberComposition::Simulink::OutputPort& outputPort_2f8e, const SFC::LocalVar& localVar_2f90, const SFC::ConditionalGroup& conditionalGroup_2f92);
	void sortOutputs();

private:
	Packets_t* _primitive_2f62;
	Packets_t* _outputPort_2f63;
	Packets_t* _localVar_2f64;
	Packets_t* _conditionalGroup_2f65;
	Packets_t _primitive_2f66;
	Packets_t _localVar_2f6f;
	Packets_t _conditionalGroup_2f78;
	class Match
	{
	public:
		CyberComposition::Simulink::Primitive primitive_2f88;
		SFC::LocalVar localVar_2f89;
		SFC::ConditionalGroup conditionalGroup_2f8a;
		CyberComposition::Simulink::OutputPort outputPort_2f8b;
	};

	std::list< Match> _matches;
};

class MakeIfVar_2f94
{
public:
	void operator()( const Packets_t& primitives_2f95, const Packets_t& compoundStatements_2f99, Packets_t& primitives_2f97, Packets_t& localVars_2f98, Packets_t& conditionalGroups_2f9b);

protected:
	bool isInputUnique( const Udm::Object& primitive_2fa3, const Udm::Object& compoundStatement_2fac);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& primitives_2f95, const Packets_t& compoundStatements_2f99);
	bool patternMatcher( const Udm::Object& primitive_2fa1, const Udm::Object& compoundStatement_2faa);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Primitive& primitive_2fbd, const SFC::LocalVar& localVar_2fbf, const SFC::ConditionalGroup& conditionalGroup_2fc1);

private:
	Packets_t* _primitive_2f9c;
	Packets_t* _localVar_2f9d;
	Packets_t* _conditionalGroup_2f9e;
	Packets_t _primitive_2f9f;
	Packets_t _compoundStatement_2fa8;
	class Match
	{
	public:
		CyberComposition::Simulink::Primitive primitive_2fb5;
		SFC::CompoundStatement compoundStatement_2fb6;
	};

	std::list< Match> _matches;
};

class GetIfAndSwitchCaseBlocks_2fc3
{
public:
	void operator()( const Packets_t& childPrimitives_2fc4, const Packets_t& systemFunctions_2fc6, Packets_t& childPrimitives_2fc8, Packets_t& systemFunctions_2fc9);

protected:
	void executeOne( const Packets_t& childPrimitives_2fc4, const Packets_t& systemFunctions_2fc6);
	bool isInputUnique( const Udm::Object& childPrimitive_2fce, const Udm::Object& systemFunction_2fd5);

private:
	Packets_t* _childPrimitive_2fca;
	Packets_t* _systemFunction_2fcb;
	Packets_t _childPrimitive_2fcc;
	Packets_t _systemFunction_2fd3;
};

class Case_2fda
{
public:
	bool operator()( const Packets_t& primitives_2fdb, const Packets_t& compoundStatements_2fde, Packets_t& primitives_2fdd, Packets_t& compoundStatements_2fe0);

protected:
	bool isInputUnique( const Udm::Object& primitive_2fe7, const Udm::Object& compoundStatement_2ff0);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, CyberComposition::Simulink::Primitive& Primitive);
	void processInputPackets( const Packets_t& primitives_2fdb, const Packets_t& compoundStatements_2fde);
	bool patternMatcher( const Udm::Object& primitive_2fe5, const Udm::Object& compoundStatement_2fee);
	void outputAppender( const CyberComposition::Simulink::Primitive& primitive_2ffd, const SFC::CompoundStatement& compoundStatement_2fff);

private:
	Packets_t* _primitive_2fe1;
	Packets_t* _compoundStatement_2fe2;
	Packets_t _primitive_2fe3;
	Packets_t _compoundStatement_2fec;
	class Match
	{
	public:
		CyberComposition::Simulink::Primitive primitive_2ff9;
		SFC::CompoundStatement compoundStatement_2ffa;
	};

	std::list< Match> _matches;
};

class AssignIfVar_3001
{
public:
	void operator()( const Packets_t& primitives_3002, const Packets_t& outputPorts_3004, const Packets_t& ifVars_3006, const Packets_t& conditionalGroups_3008);

protected:
	bool isInputUnique( const Udm::Object& primitive_300e, const Udm::Object& outputPort_3017, const Udm::Object& ifVar_3020, const Udm::Object& conditionalGroup_3029);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& primitives_3002, const Packets_t& outputPorts_3004, const Packets_t& ifVars_3006, const Packets_t& conditionalGroups_3008);
	bool patternMatcher( const Udm::Object& primitive_300c, const Udm::Object& outputPort_3015, const Udm::Object& ifVar_301e, const Udm::Object& conditionalGroup_3027);
	void effector();

private:
	Packets_t _primitive_300a;
	Packets_t _outputPort_3013;
	Packets_t _ifVar_301c;
	Packets_t _conditionalGroup_3025;
	class Match
	{
	public:
		CyberComposition::Simulink::Primitive primitive_3036;
		CyberComposition::Simulink::OutputPort outputPort_3037;
		SFC::LocalVar ifVar_3038;
		SFC::ConditionalGroup conditionalGroup_3039;
		SFC::ArgDeclBase argDeclBase_303a;
	};

	std::list< Match> _matches;
};

class ChartChildBlock_305c
{
public:
	void operator()( const Packets_t& systems_305d, const Packets_t& childCharts_305f, const Packets_t& systemFunctions_3061);

protected:
	void callMakeTrigger_4161( const Packets_t& systems_3463, const Packets_t& childSubsystems_3465, const Packets_t& systemFunctions_3467);
	void callCreateFunctionCall_4165( const Packets_t& subsystems_30b5, const Packets_t& charts_30b8, const Packets_t& compoundStatements_30bd);
	void callGetState_4169( const Packets_t& subsystems_3064, const Packets_t& charts_3067, const Packets_t& mains_306b, const Packets_t& functionCalls_306e);
	void callInputData_416e( const Packets_t& systems_31dd, const Packets_t& childCharts_31df, const Packets_t& states_31e1, const Packets_t& functions_31e3, const Packets_t& functionCalls_31e5);
	void callInputEvent_4174( const Packets_t& systems_32bb, const Packets_t& childCharts_32bd, const Packets_t& states_32bf, const Packets_t& functions_32c1, const Packets_t& functionCalls_32c3);
	void callOutputData_417a( const Packets_t& systems_30ff, const Packets_t& childCharts_3101, const Packets_t& states_3103, const Packets_t& functions_3105, const Packets_t& functionCalls_3107);
	void callOutputEvent_4180( const Packets_t& systems_33e3, const Packets_t& childCharts_33e5, const Packets_t& states_33e7, const Packets_t& functions_33e9, const Packets_t& functionCalls_33eb);
};

class GetState_3063
{
public:
	void operator()( const Packets_t& subsystems_3064, const Packets_t& charts_3067, const Packets_t& mains_306b, const Packets_t& functionCalls_306e, Packets_t& subsystems_3066, Packets_t& charts_3069, Packets_t& states_306a, Packets_t& mains_306d, Packets_t& functionCalls_3070);

protected:
	bool isInputUnique( const Udm::Object& subsystem_307a, const Udm::Object& chart_3083, const Udm::Object& main_308c, const Udm::Object& functionCall_3095);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_3064, const Packets_t& charts_3067, const Packets_t& mains_306b, const Packets_t& functionCalls_306e);
	bool patternMatcher( const Udm::Object& subsystem_3078, const Udm::Object& chart_3081, const Udm::Object& main_308a, const Udm::Object& functionCall_3093);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_30aa, const CyberComposition::Simulink::Subsystem& chart_30ac, const CyberComposition::Simulink::State& state_30ae, const SFC::Function& main_30b0, const SFC::FunctionCall& functionCall_30b2);

private:
	Packets_t* _subsystem_3071;
	Packets_t* _chart_3072;
	Packets_t* _state_3073;
	Packets_t* _main_3074;
	Packets_t* _functionCall_3075;
	Packets_t _subsystem_3076;
	Packets_t _chart_307f;
	Packets_t _main_3088;
	Packets_t _functionCall_3091;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_30a4;
		CyberComposition::Simulink::Subsystem chart_30a5;
		SFC::Function main_30a6;
		SFC::FunctionCall functionCall_30a7;
		CyberComposition::Simulink::State state_30a8;
		CyberComposition::Simulink::Primitive primitive_30a9;
	};

	std::list< Match> _matches;
};

class CreateFunctionCall_30b4
{
public:
	void operator()( const Packets_t& subsystems_30b5, const Packets_t& charts_30b8, const Packets_t& compoundStatements_30bd, Packets_t& subsystems_30b7, Packets_t& charts_30ba, Packets_t& mains_30bb, Packets_t& functionCalls_30bc);

protected:
	bool isInputUnique( const Udm::Object& subsystem_30c7, const Udm::Object& chart_30d0, const Udm::Object& compoundStatement_30d9);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_30b5, const Packets_t& charts_30b8, const Packets_t& compoundStatements_30bd);
	bool patternMatcher( const Udm::Object& subsystem_30c5, const Udm::Object& chart_30ce, const Udm::Object& compoundStatement_30d7);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_30f6, const CyberComposition::Simulink::Subsystem& chart_30f8, const SFC::Function& main_30fa, const SFC::FunctionCall& functionCall_30fc);

private:
	Packets_t* _subsystem_30bf;
	Packets_t* _chart_30c0;
	Packets_t* _main_30c1;
	Packets_t* _functionCall_30c2;
	Packets_t _subsystem_30c3;
	Packets_t _chart_30cc;
	Packets_t _compoundStatement_30d5;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_30ed;
		CyberComposition::Simulink::Subsystem chart_30ee;
		SFC::CompoundStatement compoundStatement_30ef;
		SFC::Class class_30f0;
		SFC::Struct struct_30f1;
		SFC::Arg arg_30f2;
		SFC::Function main_30f3;
	};

	std::list< Match> _matches;
};

class OutputData_30fe
{
public:
	void operator()( const Packets_t& systems_30ff, const Packets_t& childCharts_3101, const Packets_t& states_3103, const Packets_t& functions_3105, const Packets_t& functionCalls_3107, Packets_t& systems_3109, Packets_t& childCharts_310a, Packets_t& states_310b, Packets_t& functions_310c, Packets_t& functionCalls_310d);

protected:
	void callCreateOutputDataArgVal_31cc( const Packets_t& subsystems_3114, const Packets_t& charts_3118, const Packets_t& states_311a, const Packets_t& mains_311c, const Packets_t& functionCalls_3120);
	void callUpdateArgCount_31d2( const Packets_t& subsystems_3179, const Packets_t& outputPorts_317c, const Packets_t& functions_317f, const Packets_t& argVals_3181, const Packets_t& functionCalls_3184);
	void callMakeArgDeclRef_31d8( const Packets_t& systems_2cf6, const Packets_t& argPorts_2cf8, const Packets_t& argVals_2cfa);

private:
	Packets_t* _system_310e;
	Packets_t* _childChart_310f;
	Packets_t* _state_3110;
	Packets_t* _function_3111;
	Packets_t* _functionCall_3112;
};

class CreateOutputDataArgVal_3113
{
public:
	void operator()( const Packets_t& subsystems_3114, const Packets_t& charts_3118, const Packets_t& states_311a, const Packets_t& mains_311c, const Packets_t& functionCalls_3120, Packets_t& subsystems_3116, Packets_t& outputPorts_3117, Packets_t& mains_311e, Packets_t& argVals_311f, Packets_t& functionCalls_3122);

protected:
	bool isInputUnique( const Udm::Object& subsystem_312c, const Udm::Object& chart_3135, const Udm::Object& state_313e, const Udm::Object& main_3147, const Udm::Object& functionCall_3150);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::Subsystem& Chart, CyberComposition::Simulink::Data& Data, SFC::FunctionCall& FunctionCall, SFC::Function& Main, CyberComposition::Simulink::OutputPort& OutputPort, CyberComposition::Simulink::State& State, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& subsystems_3114, const Packets_t& charts_3118, const Packets_t& states_311a, const Packets_t& mains_311c, const Packets_t& functionCalls_3120);
	bool patternMatcher( const Udm::Object& subsystem_312a, const Udm::Object& chart_3133, const Udm::Object& state_313c, const Udm::Object& main_3145, const Udm::Object& functionCall_314e);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_316e, const CyberComposition::Simulink::OutputPort& outputPort_3170, const SFC::Function& main_3172, const SFC::ArgVal& argVal_3174, const SFC::FunctionCall& functionCall_3176);

private:
	Packets_t* _subsystem_3123;
	Packets_t* _outputPort_3124;
	Packets_t* _main_3125;
	Packets_t* _argVal_3126;
	Packets_t* _functionCall_3127;
	Packets_t _subsystem_3128;
	Packets_t _chart_3131;
	Packets_t _state_313a;
	Packets_t _main_3143;
	Packets_t _functionCall_314c;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_315f;
		CyberComposition::Simulink::Subsystem chart_3160;
		CyberComposition::Simulink::State state_3161;
		SFC::Function main_3162;
		SFC::FunctionCall functionCall_3163;
		CyberComposition::Simulink::OutputPort outputPort_3164;
		CyberComposition::Simulink::Data data_3165;
	};

	std::list< Match> _matches;
};

class UpdateArgCount_3178
{
public:
	void operator()( const Packets_t& subsystems_3179, const Packets_t& outputPorts_317c, const Packets_t& functions_317f, const Packets_t& argVals_3181, const Packets_t& functionCalls_3184, Packets_t& subsystems_317b, Packets_t& outputPorts_317e, Packets_t& argVals_3183);

protected:
	bool isInputUnique( const Udm::Object& subsystem_318d, const Udm::Object& outputPort_3196, const Udm::Object& function_319f, const Udm::Object& argVal_31a8, const Udm::Object& functionCall_31b1);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg, SFC::ArgVal& ArgVal, SFC::Function& Function, SFC::FunctionCall& FunctionCall, CyberComposition::Simulink::OutputPort& OutputPort, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& subsystems_3179, const Packets_t& outputPorts_317c, const Packets_t& functions_317f, const Packets_t& argVals_3181, const Packets_t& functionCalls_3184);
	bool patternMatcher( const Udm::Object& subsystem_318b, const Udm::Object& outputPort_3194, const Udm::Object& function_319d, const Udm::Object& argVal_31a6, const Udm::Object& functionCall_31af);
	void effector();
	void forwardInputs();

private:
	Packets_t* _subsystem_3186;
	Packets_t* _outputPort_3187;
	Packets_t* _argVal_3188;
	Packets_t _subsystem_3189;
	Packets_t _outputPort_3192;
	Packets_t _function_319b;
	Packets_t _argVal_31a4;
	Packets_t _functionCall_31ad;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_31c0;
		CyberComposition::Simulink::OutputPort outputPort_31c1;
		SFC::Function function_31c2;
		SFC::ArgVal argVal_31c3;
		SFC::FunctionCall functionCall_31c4;
		SFC::Arg arg_31c5;
	};

	std::list< Match> _matches;
};

class InputData_31dc
{
public:
	void operator()( const Packets_t& systems_31dd, const Packets_t& childCharts_31df, const Packets_t& states_31e1, const Packets_t& functions_31e3, const Packets_t& functionCalls_31e5, Packets_t& systems_31e7, Packets_t& childCharts_31e8, Packets_t& states_31e9, Packets_t& functions_31ea, Packets_t& functionCalls_31eb);

protected:
	void callCreateInputDataArgVal_32aa( const Packets_t& subsystems_31f2, const Packets_t& charts_31f6, const Packets_t& states_31f8, const Packets_t& mains_31fa, const Packets_t& functionCalls_31fe);
	void callUpdateArgCount_32b0( const Packets_t& subsystems_3257, const Packets_t& inputPorts_325a, const Packets_t& functions_325d, const Packets_t& argVals_325f, const Packets_t& functionCalls_3262);
	void callMakeArgDeclRef_32b6( const Packets_t& systems_2cf6, const Packets_t& argPorts_2cf8, const Packets_t& argVals_2cfa);

private:
	Packets_t* _system_31ec;
	Packets_t* _childChart_31ed;
	Packets_t* _state_31ee;
	Packets_t* _function_31ef;
	Packets_t* _functionCall_31f0;
};

class CreateInputDataArgVal_31f1
{
public:
	void operator()( const Packets_t& subsystems_31f2, const Packets_t& charts_31f6, const Packets_t& states_31f8, const Packets_t& mains_31fa, const Packets_t& functionCalls_31fe, Packets_t& subsystems_31f4, Packets_t& inputPorts_31f5, Packets_t& mains_31fc, Packets_t& argVals_31fd, Packets_t& functionCalls_3200);

protected:
	bool isInputUnique( const Udm::Object& subsystem_320a, const Udm::Object& chart_3213, const Udm::Object& state_321c, const Udm::Object& main_3225, const Udm::Object& functionCall_322e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::Subsystem& Chart, CyberComposition::Simulink::Data& Data, SFC::FunctionCall& FunctionCall, CyberComposition::Simulink::InputPort& InputPort, SFC::Function& Main, CyberComposition::Simulink::State& State, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& subsystems_31f2, const Packets_t& charts_31f6, const Packets_t& states_31f8, const Packets_t& mains_31fa, const Packets_t& functionCalls_31fe);
	bool patternMatcher( const Udm::Object& subsystem_3208, const Udm::Object& chart_3211, const Udm::Object& state_321a, const Udm::Object& main_3223, const Udm::Object& functionCall_322c);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_324c, const CyberComposition::Simulink::InputPort& inputPort_324e, const SFC::Function& main_3250, const SFC::ArgVal& argVal_3252, const SFC::FunctionCall& functionCall_3254);

private:
	Packets_t* _subsystem_3201;
	Packets_t* _inputPort_3202;
	Packets_t* _main_3203;
	Packets_t* _argVal_3204;
	Packets_t* _functionCall_3205;
	Packets_t _subsystem_3206;
	Packets_t _chart_320f;
	Packets_t _state_3218;
	Packets_t _main_3221;
	Packets_t _functionCall_322a;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_323d;
		CyberComposition::Simulink::Subsystem chart_323e;
		CyberComposition::Simulink::State state_323f;
		SFC::Function main_3240;
		SFC::FunctionCall functionCall_3241;
		CyberComposition::Simulink::InputPort inputPort_3242;
		CyberComposition::Simulink::Data data_3243;
	};

	std::list< Match> _matches;
};

class UpdateArgCount_3256
{
public:
	void operator()( const Packets_t& subsystems_3257, const Packets_t& inputPorts_325a, const Packets_t& functions_325d, const Packets_t& argVals_325f, const Packets_t& functionCalls_3262, Packets_t& subsystems_3259, Packets_t& inputPorts_325c, Packets_t& argVals_3261);

protected:
	bool isInputUnique( const Udm::Object& subsystem_326b, const Udm::Object& inputPort_3274, const Udm::Object& function_327d, const Udm::Object& argVal_3286, const Udm::Object& functionCall_328f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg, SFC::ArgVal& ArgVal, SFC::Function& Function, SFC::FunctionCall& FunctionCall, CyberComposition::Simulink::InputPort& InputPort, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& subsystems_3257, const Packets_t& inputPorts_325a, const Packets_t& functions_325d, const Packets_t& argVals_325f, const Packets_t& functionCalls_3262);
	bool patternMatcher( const Udm::Object& subsystem_3269, const Udm::Object& inputPort_3272, const Udm::Object& function_327b, const Udm::Object& argVal_3284, const Udm::Object& functionCall_328d);
	void effector();
	void forwardInputs();

private:
	Packets_t* _subsystem_3264;
	Packets_t* _inputPort_3265;
	Packets_t* _argVal_3266;
	Packets_t _subsystem_3267;
	Packets_t _inputPort_3270;
	Packets_t _function_3279;
	Packets_t _argVal_3282;
	Packets_t _functionCall_328b;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_329e;
		CyberComposition::Simulink::InputPort inputPort_329f;
		SFC::Function function_32a0;
		SFC::ArgVal argVal_32a1;
		SFC::FunctionCall functionCall_32a2;
		SFC::Arg arg_32a3;
	};

	std::list< Match> _matches;
};

class InputEvent_32ba
{
public:
	void operator()( const Packets_t& systems_32bb, const Packets_t& childCharts_32bd, const Packets_t& states_32bf, const Packets_t& functions_32c1, const Packets_t& functionCalls_32c3, Packets_t& systems_32c5, Packets_t& childCharts_32c6, Packets_t& states_32c7, Packets_t& functions_32c8, Packets_t& functionCalls_32c9);

protected:
	void callGetOldVar_33d6( const Packets_t& blocks_32d1, const Packets_t& mains_32d3, const Packets_t& functionCalls_32d6);
	void callMatrixInputEvent_33da( const Packets_t& oldvars_330b, const Packets_t& functions_330d, const Packets_t& functionCalls_330f);
	void callStructInputEvent_33de( const Packets_t& oldvars_3355, const Packets_t& functions_3357, const Packets_t& functionCalls_3359);

private:
	Packets_t* _system_32ca;
	Packets_t* _childChart_32cb;
	Packets_t* _state_32cc;
	Packets_t* _function_32cd;
	Packets_t* _functionCall_32ce;
};

class GetOldVar_32cf
{
public:
	void operator()( const Packets_t& blocks_32d1, const Packets_t& mains_32d3, const Packets_t& functionCalls_32d6, Packets_t& localVars_32d0, Packets_t& mains_32d5, Packets_t& functionCalls_32d8);

protected:
	bool isInputUnique( const Udm::Object& block_32e0, const Udm::Object& main_32e9, const Udm::Object& functionCall_32f2);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& blocks_32d1, const Packets_t& mains_32d3, const Packets_t& functionCalls_32d6);
	bool patternMatcher( const Udm::Object& block_32de, const Udm::Object& main_32e7, const Udm::Object& functionCall_32f0);
	void effector();
	void outputAppender( const SFC::LocalVar& localVar_3304, const SFC::Function& main_3306, const SFC::FunctionCall& functionCall_3308);

private:
	Packets_t* _localVar_32d9;
	Packets_t* _main_32da;
	Packets_t* _functionCall_32db;
	Packets_t _block_32dc;
	Packets_t _main_32e5;
	Packets_t _functionCall_32ee;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem block_32ff;
		SFC::Function main_3300;
		SFC::FunctionCall functionCall_3301;
		SFC::LocalVar localVar_3302;
		CyberComposition::Simulink::TriggerPort triggerPort_3303;
	};

	std::list< Match> _matches;
};

class MatrixInputEvent_330a
{
public:
	void operator()( const Packets_t& oldvars_330b, const Packets_t& functions_330d, const Packets_t& functionCalls_330f, Packets_t& oldvars_3311, Packets_t& functions_3312, Packets_t& functionCalls_3313);

protected:
	void callMatrixInputEventArgVal_3350( const Packets_t& localVars_3318, const Packets_t& functions_331a, const Packets_t& functionCalls_331c);

private:
	Packets_t* _oldvar_3314;
	Packets_t* _function_3315;
	Packets_t* _functionCall_3316;
};

class MatrixInputEventArgVal_3317
{
public:
	void operator()( const Packets_t& localVars_3318, const Packets_t& functions_331a, const Packets_t& functionCalls_331c);

protected:
	bool isInputUnique( const Udm::Object& localVar_3322, const Udm::Object& function_332b, const Udm::Object& functionCall_3334);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg, SFC::DT& DT, SFC::Function& Function, SFC::FunctionCall& FunctionCall, SFC::LocalVar& LocalVar, SFC::LocalVar& TriggerVar);
	void processInputPackets( const Packets_t& localVars_3318, const Packets_t& functions_331a, const Packets_t& functionCalls_331c);
	bool patternMatcher( const Udm::Object& localVar_3320, const Udm::Object& function_3329, const Udm::Object& functionCall_3332);
	void effector();

private:
	Packets_t _localVar_331e;
	Packets_t _function_3327;
	Packets_t _functionCall_3330;
	class Match
	{
	public:
		SFC::LocalVar localVar_3342;
		SFC::Function function_3343;
		SFC::FunctionCall functionCall_3344;
		SFC::DT dT_3345;
		SFC::LocalVar triggerVar_3346;
		SFC::Arg arg_3347;
	};

	std::list< Match> _matches;
};

class StructInputEvent_3354
{
public:
	void operator()( const Packets_t& oldvars_3355, const Packets_t& functions_3357, const Packets_t& functionCalls_3359);

protected:
	void callStructInputEventArgVal_33ce( const Packets_t& localVars_335c, const Packets_t& mains_335e, const Packets_t& functionCalls_3362);
	void callUpdateArgCount_33d2( const Packets_t& functions_339e, const Packets_t& argVals_33a0, const Packets_t& functionCalls_33a2);
};

class StructInputEventArgVal_335b
{
public:
	void operator()( const Packets_t& localVars_335c, const Packets_t& mains_335e, const Packets_t& functionCalls_3362, Packets_t& mains_3360, Packets_t& argVals_3361, Packets_t& functionCalls_3364);

protected:
	bool isInputUnique( const Udm::Object& localVar_336c, const Udm::Object& main_3375, const Udm::Object& functionCall_337e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& localVars_335c, const Packets_t& mains_335e, const Packets_t& functionCalls_3362);
	bool patternMatcher( const Udm::Object& localVar_336a, const Udm::Object& main_3373, const Udm::Object& functionCall_337c);
	void effector();
	void outputAppender( const SFC::Function& main_3397, const SFC::ArgVal& argVal_3399, const SFC::FunctionCall& functionCall_339b);

private:
	Packets_t* _main_3365;
	Packets_t* _argVal_3366;
	Packets_t* _functionCall_3367;
	Packets_t _localVar_3368;
	Packets_t _main_3371;
	Packets_t _functionCall_337a;
	class Match
	{
	public:
		SFC::LocalVar localVar_338d;
		SFC::Function main_338e;
		SFC::FunctionCall functionCall_338f;
		SFC::LocalVar triggerVar_3390;
		SFC::Struct struct_3391;
		SFC::LocalVar member_3392;
	};

	std::list< Match> _matches;
};

class UpdateArgCount_339d
{
public:
	void operator()( const Packets_t& functions_339e, const Packets_t& argVals_33a0, const Packets_t& functionCalls_33a2);

protected:
	bool isInputUnique( const Udm::Object& function_33a8, const Udm::Object& argVal_33b1, const Udm::Object& functionCall_33ba);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg, SFC::ArgVal& ArgVal, SFC::Function& Function, SFC::FunctionCall& FunctionCall);
	void processInputPackets( const Packets_t& functions_339e, const Packets_t& argVals_33a0, const Packets_t& functionCalls_33a2);
	bool patternMatcher( const Udm::Object& function_33a6, const Udm::Object& argVal_33af, const Udm::Object& functionCall_33b8);
	void effector();

private:
	Packets_t _function_33a4;
	Packets_t _argVal_33ad;
	Packets_t _functionCall_33b6;
	class Match
	{
	public:
		SFC::Function function_33c6;
		SFC::ArgVal argVal_33c7;
		SFC::FunctionCall functionCall_33c8;
		SFC::Arg arg_33c9;
	};

	std::list< Match> _matches;
};

class OutputEvent_33e2
{
public:
	void operator()( const Packets_t& systems_33e3, const Packets_t& childCharts_33e5, const Packets_t& states_33e7, const Packets_t& functions_33e9, const Packets_t& functionCalls_33eb);

protected:
	void callCreateOutputEventArgVal_3452( const Packets_t& subsystems_33ee, const Packets_t& charts_33f2, const Packets_t& states_33f4, const Packets_t& mains_33f6, const Packets_t& functionCalls_33fa);
	void callUpdateArgCount_3458( const Packets_t& subsystems_3179, const Packets_t& outputPorts_317c, const Packets_t& functions_317f, const Packets_t& argVals_3181, const Packets_t& functionCalls_3184);
	void callMakeArgDeclRef_345e( const Packets_t& systems_2cf6, const Packets_t& argPorts_2cf8, const Packets_t& argVals_2cfa);
};

class CreateOutputEventArgVal_33ed
{
public:
	void operator()( const Packets_t& subsystems_33ee, const Packets_t& charts_33f2, const Packets_t& states_33f4, const Packets_t& mains_33f6, const Packets_t& functionCalls_33fa, Packets_t& subsystems_33f0, Packets_t& outputPorts_33f1, Packets_t& mains_33f8, Packets_t& argVals_33f9, Packets_t& functionCalls_33fc);

protected:
	bool isInputUnique( const Udm::Object& subsystem_3406, const Udm::Object& chart_340f, const Udm::Object& state_3418, const Udm::Object& main_3421, const Udm::Object& functionCall_342a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::Subsystem& Chart, CyberComposition::Simulink::Event& Event, SFC::FunctionCall& FunctionCall, SFC::Function& Main, CyberComposition::Simulink::OutputPort& OutputPort, CyberComposition::Simulink::State& State, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& subsystems_33ee, const Packets_t& charts_33f2, const Packets_t& states_33f4, const Packets_t& mains_33f6, const Packets_t& functionCalls_33fa);
	bool patternMatcher( const Udm::Object& subsystem_3404, const Udm::Object& chart_340d, const Udm::Object& state_3416, const Udm::Object& main_341f, const Udm::Object& functionCall_3428);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_3448, const CyberComposition::Simulink::OutputPort& outputPort_344a, const SFC::Function& main_344c, const SFC::ArgVal& argVal_344e, const SFC::FunctionCall& functionCall_3450);

private:
	Packets_t* _subsystem_33fd;
	Packets_t* _outputPort_33fe;
	Packets_t* _main_33ff;
	Packets_t* _argVal_3400;
	Packets_t* _functionCall_3401;
	Packets_t _subsystem_3402;
	Packets_t _chart_340b;
	Packets_t _state_3414;
	Packets_t _main_341d;
	Packets_t _functionCall_3426;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_3439;
		CyberComposition::Simulink::Subsystem chart_343a;
		CyberComposition::Simulink::State state_343b;
		SFC::Function main_343c;
		SFC::FunctionCall functionCall_343d;
		CyberComposition::Simulink::OutputPort outputPort_343e;
		CyberComposition::Simulink::Event event_343f;
	};

	std::list< Match> _matches;
};

class MakeTrigger_3462
{
public:
	void operator()( const Packets_t& systems_3463, const Packets_t& childSubsystems_3465, const Packets_t& systemFunctions_3467, Packets_t& systems_3469, Packets_t& childSubsystems_346a, Packets_t& compoundStatements_346b);

protected:
	void callIsTriggered_4159( const Packets_t& systems_40c4, const Packets_t& childSubsystems_40c6, const Packets_t& systemFunctions_40c8);
	void callCreateTriggering_415d( const Packets_t& systems_3470, const Packets_t& childSubsystems_3472, const Packets_t& systemFunctions_3474);

private:
	Packets_t* _system_346c;
	Packets_t* _childSubsystem_346d;
	Packets_t* _compoundStatement_346e;
};

class CreateTriggering_346f
{
public:
	void operator()( const Packets_t& systems_3470, const Packets_t& childSubsystems_3472, const Packets_t& systemFunctions_3474, Packets_t& systems_3476, Packets_t& childSubsystems_3477, Packets_t& conditionalBlocks_3478);

protected:
	void callGetTriggerPort_40b5( const Packets_t& subsystems_403c, const Packets_t& blocks_403f, const Packets_t& mains_4042);
	void callAssignCondValLocalVar_40b9( const Packets_t& subsystems_4075, const Packets_t& triggerPorts_4078, const Packets_t& functions_407d);
	void callMakeTrigger_40bd( const Packets_t& systems_347d, const Packets_t& childSubsystemTriggers_347f, const Packets_t& argdecls_3481, const Packets_t& dts_3483, const Packets_t& functions_3485);

private:
	Packets_t* _system_3479;
	Packets_t* _childSubsystem_347a;
	Packets_t* _conditionalBlock_347b;
};

class MakeTrigger_347c
{
public:
	void operator()( const Packets_t& systems_347d, const Packets_t& childSubsystemTriggers_347f, const Packets_t& argdecls_3481, const Packets_t& dts_3483, const Packets_t& functions_3485, Packets_t& systems_3487, Packets_t& childSubsystems_3488, Packets_t& conditionals_3489);

protected:
	void callAddStateVar_401f( const Packets_t& subsystems_37a0, const Packets_t& triggerPorts_37a3, const Packets_t& argDeclBases_37a6, const Packets_t& dTs_37aa, const Packets_t& functions_37ae);
	void callAddTriggerVars_4025( const Packets_t& systems_38fa, const Packets_t& childsystems_38fc, const Packets_t& argdecls_38fe, const Packets_t& dts_3900, const Packets_t& oldvals_3902, const Packets_t& contexts_3904, const Packets_t& functions_3906);
	void callSaveState_402d( const Packets_t& systems_380e, const Packets_t& childsystems_3810, const Packets_t& argdecls_3812, const Packets_t& dts_3814, const Packets_t& oldvals_3816, const Packets_t& contexts_3818, const Packets_t& functions_381a);
	void callMakeCondition_4035( const Packets_t& systems_348e, const Packets_t& childSubsystems_3490, const Packets_t& dts_3492, const Packets_t& oldvals_3494, const Packets_t& functions_3496);

private:
	Packets_t* _system_348a;
	Packets_t* _childSubsystem_348b;
	Packets_t* _conditional_348c;
};

class MakeCondition_348d
{
public:
	void operator()( const Packets_t& systems_348e, const Packets_t& childSubsystems_3490, const Packets_t& dts_3492, const Packets_t& oldvals_3494, const Packets_t& functions_3496, Packets_t& systems_3498, Packets_t& childSubsystems_3499, Packets_t& conditionals_349a);

protected:
	void callCreateConditional_3791( const Packets_t& subsystems_349f, const Packets_t& charts_34a2, const Packets_t& dTs_34a5, const Packets_t& localVars_34a8, const Packets_t& functions_34ac);
	void callSetMatrixCondition_3797( const Packets_t& dts_34fc, const Packets_t& triggervars_34fe, const Packets_t& conditions_3500);
	void callSetStructCondition_379b( const Packets_t& dts_353a, const Packets_t& oldvals_353c, const Packets_t& conditions_353e);

private:
	Packets_t* _system_349b;
	Packets_t* _childSubsystem_349c;
	Packets_t* _conditional_349d;
};

class CreateConditional_349e
{
public:
	void operator()( const Packets_t& subsystems_349f, const Packets_t& charts_34a2, const Packets_t& dTs_34a5, const Packets_t& localVars_34a8, const Packets_t& functions_34ac, Packets_t& subsystems_34a1, Packets_t& charts_34a4, Packets_t& dTs_34a7, Packets_t& triggerVars_34aa, Packets_t& userCodes_34ab, Packets_t& conditionalBlocks_34ae);

protected:
	bool isInputUnique( const Udm::Object& subsystem_34b9, const Udm::Object& chart_34c2, const Udm::Object& dT_34cb, const Udm::Object& localVar_34d4, const Udm::Object& function_34dd);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_349f, const Packets_t& charts_34a2, const Packets_t& dTs_34a5, const Packets_t& localVars_34a8, const Packets_t& functions_34ac);
	bool patternMatcher( const Udm::Object& subsystem_34b7, const Udm::Object& chart_34c0, const Udm::Object& dT_34c9, const Udm::Object& localVar_34d2, const Udm::Object& function_34db);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_34ef, const CyberComposition::Simulink::Subsystem& chart_34f1, const SFC::DT& dT_34f3, const SFC::LocalVar& triggerVar_34f5, const SFC::UserCode& userCode_34f7, const SFC::ConditionalBlock& conditionalBlock_34f9);

private:
	Packets_t* _subsystem_34af;
	Packets_t* _chart_34b0;
	Packets_t* _dT_34b1;
	Packets_t* _triggerVar_34b2;
	Packets_t* _userCode_34b3;
	Packets_t* _conditionalBlock_34b4;
	Packets_t _subsystem_34b5;
	Packets_t _chart_34be;
	Packets_t _dT_34c7;
	Packets_t _localVar_34d0;
	Packets_t _function_34d9;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_34e7;
		CyberComposition::Simulink::Subsystem chart_34e8;
		SFC::DT dT_34e9;
		SFC::LocalVar localVar_34ea;
		SFC::Function function_34eb;
		SFC::LocalVar triggerVar_34ec;
	};

	std::list< Match> _matches;
};

class SetMatrixCondition_34fb
{
public:
	void operator()( const Packets_t& dts_34fc, const Packets_t& triggervars_34fe, const Packets_t& conditions_3500, Packets_t& dts_3502, Packets_t& triggervars_3503, Packets_t& conditions_3504);

protected:
	void callMatrixCondition_3535( const Packets_t& dTs_3509, const Packets_t& triggerVars_350b, const Packets_t& conditions_350d);

private:
	Packets_t* _dt_3505;
	Packets_t* _triggervar_3506;
	Packets_t* _condition_3507;
};

class MatrixCondition_3508
{
public:
	void operator()( const Packets_t& dTs_3509, const Packets_t& triggerVars_350b, const Packets_t& conditions_350d);

protected:
	bool isInputUnique( const Udm::Object& dT_3513, const Udm::Object& triggerVar_351c, const Udm::Object& condition_3525);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::UserCode& Condition, SFC::DT& DT, SFC::LocalVar& TriggerVar);
	void processInputPackets( const Packets_t& dTs_3509, const Packets_t& triggerVars_350b, const Packets_t& conditions_350d);
	bool patternMatcher( const Udm::Object& dT_3511, const Udm::Object& triggerVar_351a, const Udm::Object& condition_3523);
	void effector();

private:
	Packets_t _dT_350f;
	Packets_t _triggerVar_3518;
	Packets_t _condition_3521;
	class Match
	{
	public:
		SFC::DT dT_352e;
		SFC::LocalVar triggerVar_352f;
		SFC::UserCode condition_3530;
	};

	std::list< Match> _matches;
};

class SetStructCondition_3539
{
public:
	void operator()( const Packets_t& dts_353a, const Packets_t& oldvals_353c, const Packets_t& conditions_353e);

protected:
	void callStartCondition_3784( const Packets_t& structs_374f, const Packets_t& triggerVars_3752, const Packets_t& userCodes_3755);
	void callgetStructMembers_3788( const Packets_t& structs_3541, const Packets_t& triggerVars_3545, const Packets_t& unaryExprss_3548);
	void callPlaceMember_378c( const Packets_t& structs_357e, const Packets_t& members_3580, const Packets_t& trigvars_3582, const Packets_t& condexprs_3584);
};

class GetStructMembers_3540
{
public:
	void operator()( const Packets_t& structs_3541, const Packets_t& triggerVars_3545, const Packets_t& unaryExprss_3548, Packets_t& structs_3543, Packets_t& members_3544, Packets_t& triggerVars_3547, Packets_t& unaryExprss_354a);

protected:
	bool isInputUnique( const Udm::Object& struct_3553, const Udm::Object& triggerVar_355c, const Udm::Object& unaryExprs_3565);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& structs_3541, const Packets_t& triggerVars_3545, const Packets_t& unaryExprss_3548);
	bool patternMatcher( const Udm::Object& struct_3551, const Udm::Object& triggerVar_355a, const Udm::Object& unaryExprs_3563);
	void effector();
	void outputAppender( const SFC::Struct& struct_3575, const SFC::LocalVar& member_3577, const SFC::LocalVar& triggerVar_3579, const SFC::UnaryExprs& unaryExprs_357b);
	void sortOutputs();

private:
	Packets_t* _struct_354b;
	Packets_t* _member_354c;
	Packets_t* _triggerVar_354d;
	Packets_t* _unaryExprs_354e;
	Packets_t _struct_354f;
	Packets_t _triggerVar_3558;
	Packets_t _unaryExprs_3561;
	class Match
	{
	public:
		SFC::Struct struct_3571;
		SFC::LocalVar triggerVar_3572;
		SFC::UnaryExprs unaryExprs_3573;
		SFC::LocalVar member_3574;
	};

	std::list< Match> _matches;
};

class PlaceMember_357d
{
public:
	void operator()( const Packets_t& structs_357e, const Packets_t& members_3580, const Packets_t& trigvars_3582, const Packets_t& condexprs_3584);

protected:
	void executeOne( const Packets_t& structs_357e, const Packets_t& members_3580, const Packets_t& trigvars_3582, const Packets_t& condexprs_3584);
	bool isInputUnique( const Udm::Object& struct_3588, const Udm::Object& member_358f, const Udm::Object& trigvar_3596, const Udm::Object& condexpr_359d);
	void callTriggerTest_373f( const Packets_t& structs_35f0, const Packets_t& members_35f2, const Packets_t& trigvars_35f4, const Packets_t& condexprs_35f6);
	void callTraverseBinary_3744( const Packets_t& structs_35a3, const Packets_t& members_35a6, const Packets_t& triggerVars_35a9, const Packets_t& unaryExprss_35ac);
	void callAddTrigger_3749( const Packets_t& structs_36b3, const Packets_t& members_36b5, const Packets_t& trigvars_36b7, const Packets_t& condexprs_36b9);

private:
	Packets_t _struct_3586;
	Packets_t _member_358d;
	Packets_t _trigvar_3594;
	Packets_t _condexpr_359b;
};

class TraverseBinary_35a2
{
public:
	void operator()( const Packets_t& structs_35a3, const Packets_t& members_35a6, const Packets_t& triggerVars_35a9, const Packets_t& unaryExprss_35ac, Packets_t& structs_35a5, Packets_t& members_35a8, Packets_t& triggerVars_35ab, Packets_t& unaryExprss_35ae);

protected:
	bool isInputUnique( const Udm::Object& struct_35b7, const Udm::Object& member_35c0, const Udm::Object& triggerVar_35c9, const Udm::Object& unaryExprs_35d2);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& structs_35a3, const Packets_t& members_35a6, const Packets_t& triggerVars_35a9, const Packets_t& unaryExprss_35ac);
	bool patternMatcher( const Udm::Object& struct_35b5, const Udm::Object& member_35be, const Udm::Object& triggerVar_35c7, const Udm::Object& unaryExprs_35d0);
	void effector();
	void outputAppender( const SFC::Struct& struct_35e7, const SFC::LocalVar& member_35e9, const SFC::LocalVar& triggerVar_35eb, const SFC::UnaryExprs& unaryExprs_35ed);

private:
	Packets_t* _struct_35af;
	Packets_t* _member_35b0;
	Packets_t* _triggerVar_35b1;
	Packets_t* _unaryExprs_35b2;
	Packets_t _struct_35b3;
	Packets_t _member_35bc;
	Packets_t _triggerVar_35c5;
	Packets_t _unaryExprs_35ce;
	class Match
	{
	public:
		SFC::Struct struct_35e1;
		SFC::LocalVar member_35e2;
		SFC::LocalVar triggerVar_35e3;
		SFC::UnaryExprs unaryExprs_35e4;
		SFC::BinaryExprs binaryExprs_35e5;
		SFC::UnaryExprs unaryExprs_35e6;
	};

	std::list< Match> _matches;
};

class TriggerTest_35ef
{
public:
	void operator()( const Packets_t& structs_35f0, const Packets_t& members_35f2, const Packets_t& trigvars_35f4, const Packets_t& condexprs_35f6, Packets_t& structs_35f8, Packets_t& members_35f9, Packets_t& trigvars_35fa, Packets_t& condexprs_35fb, Packets_t& structs_35fc, Packets_t& members_35fd, Packets_t& trigvars_35fe, Packets_t& condexprs_35ff);

protected:
	void executeOne( const Packets_t& structs_35f0, const Packets_t& members_35f2, const Packets_t& trigvars_35f4, const Packets_t& condexprs_35f6);
	bool isInputUnique( const Udm::Object& struct_360a, const Udm::Object& member_3611, const Udm::Object& trigvar_3618, const Udm::Object& condexpr_361f);

private:
	Packets_t* _struct_3600;
	Packets_t* _member_3601;
	Packets_t* _trigvar_3602;
	Packets_t* _condexpr_3603;
	Packets_t* _struct_3604;
	Packets_t* _member_3605;
	Packets_t* _trigvar_3606;
	Packets_t* _condexpr_3607;
	Packets_t _struct_3608;
	Packets_t _member_360f;
	Packets_t _trigvar_3616;
	Packets_t _condexpr_361d;
};

class HasBinaryExprs_3624
{
public:
	bool operator()( const Packets_t& structs_3625, const Packets_t& members_3628, const Packets_t& triggerVars_362b, const Packets_t& unaryExprss_362e, Packets_t& structs_3627, Packets_t& members_362a, Packets_t& triggerVars_362d, Packets_t& unaryExprss_3630);

protected:
	bool isInputUnique( const Udm::Object& struct_3639, const Udm::Object& member_3642, const Udm::Object& triggerVar_364b, const Udm::Object& unaryExprs_3654);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& structs_3625, const Packets_t& members_3628, const Packets_t& triggerVars_362b, const Packets_t& unaryExprss_362e);
	bool patternMatcher( const Udm::Object& struct_3637, const Udm::Object& member_3640, const Udm::Object& triggerVar_3649, const Udm::Object& unaryExprs_3652);
	void outputAppender( const SFC::Struct& struct_3665, const SFC::LocalVar& member_3667, const SFC::LocalVar& triggerVar_3669, const SFC::UnaryExprs& unaryExprs_366b);

private:
	Packets_t* _struct_3631;
	Packets_t* _member_3632;
	Packets_t* _triggerVar_3633;
	Packets_t* _unaryExprs_3634;
	Packets_t _struct_3635;
	Packets_t _member_363e;
	Packets_t _triggerVar_3647;
	Packets_t _unaryExprs_3650;
	class Match
	{
	public:
		SFC::Struct struct_3660;
		SFC::LocalVar member_3661;
		SFC::LocalVar triggerVar_3662;
		SFC::UnaryExprs unaryExprs_3663;
		SFC::BinaryExprs binaryExprs_3664;
	};

	std::list< Match> _matches;
};

class Otherwise_366d
{
public:
	bool operator()( const Packets_t& structs_366e, const Packets_t& members_3671, const Packets_t& triggerVars_3674, const Packets_t& unaryExprss_3677, Packets_t& structs_3670, Packets_t& members_3673, Packets_t& triggerVars_3676, Packets_t& unaryExprss_3679);

protected:
	bool isInputUnique( const Udm::Object& struct_3682, const Udm::Object& member_368b, const Udm::Object& triggerVar_3694, const Udm::Object& unaryExprs_369d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& structs_366e, const Packets_t& members_3671, const Packets_t& triggerVars_3674, const Packets_t& unaryExprss_3677);
	bool patternMatcher( const Udm::Object& struct_3680, const Udm::Object& member_3689, const Udm::Object& triggerVar_3692, const Udm::Object& unaryExprs_369b);
	void outputAppender( const SFC::Struct& struct_36aa, const SFC::LocalVar& member_36ac, const SFC::LocalVar& triggerVar_36ae, const SFC::UnaryExprs& unaryExprs_36b0);

private:
	Packets_t* _struct_367a;
	Packets_t* _member_367b;
	Packets_t* _triggerVar_367c;
	Packets_t* _unaryExprs_367d;
	Packets_t _struct_367e;
	Packets_t _member_3687;
	Packets_t _triggerVar_3690;
	Packets_t _unaryExprs_3699;
	class Match
	{
	public:
		SFC::Struct struct_36a6;
		SFC::LocalVar member_36a7;
		SFC::LocalVar triggerVar_36a8;
		SFC::UnaryExprs unaryExprs_36a9;
	};

	std::list< Match> _matches;
};

class AddTrigger_36b2
{
public:
	void operator()( const Packets_t& structs_36b3, const Packets_t& members_36b5, const Packets_t& trigvars_36b7, const Packets_t& condexprs_36b9);

protected:
	void callAddNextTrigger_3735( const Packets_t& structs_36bc, const Packets_t& members_36be, const Packets_t& triggerVars_36c0, const Packets_t& unaryExprss_36c2);
	void callAddLastTrigger_373a( const Packets_t& structs_36fa, const Packets_t& members_36fc, const Packets_t& triggerVars_36fe, const Packets_t& unaryExprss_3700);
};

class AddNextTrigger_36bb
{
public:
	void operator()( const Packets_t& structs_36bc, const Packets_t& members_36be, const Packets_t& triggerVars_36c0, const Packets_t& unaryExprss_36c2);

protected:
	bool isInputUnique( const Udm::Object& struct_36c8, const Udm::Object& member_36d1, const Udm::Object& triggerVar_36da, const Udm::Object& unaryExprs_36e3);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::LocalVar& Member, SFC::Struct& Struct, SFC::LocalVar& TriggerVar, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& structs_36bc, const Packets_t& members_36be, const Packets_t& triggerVars_36c0, const Packets_t& unaryExprss_36c2);
	bool patternMatcher( const Udm::Object& struct_36c6, const Udm::Object& member_36cf, const Udm::Object& triggerVar_36d8, const Udm::Object& unaryExprs_36e1);
	void effector();

private:
	Packets_t _struct_36c4;
	Packets_t _member_36cd;
	Packets_t _triggerVar_36d6;
	Packets_t _unaryExprs_36df;
	class Match
	{
	public:
		SFC::Struct struct_36ec;
		SFC::LocalVar member_36ed;
		SFC::LocalVar triggerVar_36ee;
		SFC::UnaryExprs unaryExprs_36ef;
	};

	std::list< Match> _matches;
};

class AddLastTrigger_36f9
{
public:
	void operator()( const Packets_t& structs_36fa, const Packets_t& members_36fc, const Packets_t& triggerVars_36fe, const Packets_t& unaryExprss_3700);

protected:
	bool isInputUnique( const Udm::Object& struct_3706, const Udm::Object& member_370f, const Udm::Object& triggerVar_3718, const Udm::Object& unaryExprs_3721);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::LocalVar& Member, SFC::Struct& Struct, SFC::LocalVar& TriggerVar, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& structs_36fa, const Packets_t& members_36fc, const Packets_t& triggerVars_36fe, const Packets_t& unaryExprss_3700);
	bool patternMatcher( const Udm::Object& struct_3704, const Udm::Object& member_370d, const Udm::Object& triggerVar_3716, const Udm::Object& unaryExprs_371f);
	void effector();

private:
	Packets_t _struct_3702;
	Packets_t _member_370b;
	Packets_t _triggerVar_3714;
	Packets_t _unaryExprs_371d;
	class Match
	{
	public:
		SFC::Struct struct_372a;
		SFC::LocalVar member_372b;
		SFC::LocalVar triggerVar_372c;
		SFC::UnaryExprs unaryExprs_372d;
	};

	std::list< Match> _matches;
};

class StartCondition_374e
{
public:
	void operator()( const Packets_t& structs_374f, const Packets_t& triggerVars_3752, const Packets_t& userCodes_3755, Packets_t& structs_3751, Packets_t& triggerVars_3754, Packets_t& unaryExprss_3757);

protected:
	bool isInputUnique( const Udm::Object& struct_375f, const Udm::Object& triggerVar_3768, const Udm::Object& userCode_3771);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& structs_374f, const Packets_t& triggerVars_3752, const Packets_t& userCodes_3755);
	bool patternMatcher( const Udm::Object& struct_375d, const Udm::Object& triggerVar_3766, const Udm::Object& userCode_376f);
	void effector();
	void outputAppender( const SFC::Struct& struct_377e, const SFC::LocalVar& triggerVar_3780, const SFC::UnaryExprs& unaryExprs_3782);

private:
	Packets_t* _struct_3758;
	Packets_t* _triggerVar_3759;
	Packets_t* _unaryExprs_375a;
	Packets_t _struct_375b;
	Packets_t _triggerVar_3764;
	Packets_t _userCode_376d;
	class Match
	{
	public:
		SFC::Struct struct_377a;
		SFC::LocalVar triggerVar_377b;
		SFC::UserCode userCode_377c;
	};

	std::list< Match> _matches;
};

class AddStateVar_379f
{
public:
	void operator()( const Packets_t& subsystems_37a0, const Packets_t& triggerPorts_37a3, const Packets_t& argDeclBases_37a6, const Packets_t& dTs_37aa, const Packets_t& functions_37ae, Packets_t& subsystems_37a2, Packets_t& charts_37a5, Packets_t& argDeclBases_37a8, Packets_t& localVars_37a9, Packets_t& dTs_37ac, Packets_t& args_37ad, Packets_t& functions_37b0);

protected:
	bool isInputUnique( const Udm::Object& subsystem_37bc, const Udm::Object& triggerPort_37c5, const Udm::Object& argDeclBase_37ce, const Udm::Object& dT_37d7, const Udm::Object& function_37e0);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_37a0, const Packets_t& triggerPorts_37a3, const Packets_t& argDeclBases_37a6, const Packets_t& dTs_37aa, const Packets_t& functions_37ae);
	bool patternMatcher( const Udm::Object& subsystem_37ba, const Udm::Object& triggerPort_37c3, const Udm::Object& argDeclBase_37cc, const Udm::Object& dT_37d5, const Udm::Object& function_37de);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_37ff, const CyberComposition::Simulink::Subsystem& chart_3801, const SFC::ArgDeclBase& argDeclBase_3803, const SFC::LocalVar& localVar_3805, const SFC::DT& dT_3807, const SFC::Arg& arg_3809, const SFC::Function& function_380b);

private:
	Packets_t* _subsystem_37b1;
	Packets_t* _chart_37b2;
	Packets_t* _argDeclBase_37b3;
	Packets_t* _localVar_37b4;
	Packets_t* _dT_37b5;
	Packets_t* _arg_37b6;
	Packets_t* _function_37b7;
	Packets_t _subsystem_37b8;
	Packets_t _triggerPort_37c1;
	Packets_t _argDeclBase_37ca;
	Packets_t _dT_37d3;
	Packets_t _function_37dc;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_37f5;
		CyberComposition::Simulink::TriggerPort triggerPort_37f6;
		SFC::ArgDeclBase argDeclBase_37f7;
		SFC::DT dT_37f8;
		SFC::Function function_37f9;
		CyberComposition::Simulink::Subsystem chart_37fa;
		SFC::Arg arg_37fb;
		SFC::Struct struct_37fc;
		SFC::Class class_37fd;
	};

	std::list< Match> _matches;
};

class SaveState_380d
{
public:
	void operator()( const Packets_t& systems_380e, const Packets_t& childsystems_3810, const Packets_t& argdecls_3812, const Packets_t& dts_3814, const Packets_t& oldvals_3816, const Packets_t& contexts_3818, const Packets_t& functions_381a, Packets_t& systems_381c, Packets_t& childsystems_381d, Packets_t& dts_381e, Packets_t& oldvals_381f, Packets_t& functions_3820);

protected:
	void callSaveMatrixState_38ed( const Packets_t& argdecls_3827, const Packets_t& dts_3829, const Packets_t& oldvals_382b, const Packets_t& contexts_382d, const Packets_t& functions_382f);
	void callSaveStructState_38f3( const Packets_t& argdecls_388e, const Packets_t& dts_3890, const Packets_t& oldvals_3892, const Packets_t& contexts_3894, const Packets_t& functions_3896);

private:
	Packets_t* _system_3821;
	Packets_t* _childsystem_3822;
	Packets_t* _dt_3823;
	Packets_t* _oldval_3824;
	Packets_t* _function_3825;
};

class SaveMatrixState_3826
{
public:
	void operator()( const Packets_t& argdecls_3827, const Packets_t& dts_3829, const Packets_t& oldvals_382b, const Packets_t& contexts_382d, const Packets_t& functions_382f, Packets_t& argdecls_3831, Packets_t& dts_3832, Packets_t& oldvals_3833, Packets_t& contexts_3834, Packets_t& functions_3835);

protected:
	void callSaveMatrixState_3887( const Packets_t& argDeclBases_383c, const Packets_t& dTs_383e, const Packets_t& localVars_3840, const Packets_t& args_3842, const Packets_t& functions_3844);

private:
	Packets_t* _argdecl_3836;
	Packets_t* _dt_3837;
	Packets_t* _oldval_3838;
	Packets_t* _context_3839;
	Packets_t* _function_383a;
};

class SaveMatrixState_383b
{
public:
	void operator()( const Packets_t& argDeclBases_383c, const Packets_t& dTs_383e, const Packets_t& localVars_3840, const Packets_t& args_3842, const Packets_t& functions_3844);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_384a, const Udm::Object& dT_3853, const Udm::Object& localVar_385c, const Udm::Object& arg_3865, const Udm::Object& function_386e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg, SFC::ArgDeclBase& ArgDeclBase, SFC::DT& DT, SFC::Function& Function, SFC::LocalVar& LocalVar);
	void processInputPackets( const Packets_t& argDeclBases_383c, const Packets_t& dTs_383e, const Packets_t& localVars_3840, const Packets_t& args_3842, const Packets_t& functions_3844);
	bool patternMatcher( const Udm::Object& argDeclBase_3848, const Udm::Object& dT_3851, const Udm::Object& localVar_385a, const Udm::Object& arg_3863, const Udm::Object& function_386c);
	void effector();

private:
	Packets_t _argDeclBase_3846;
	Packets_t _dT_384f;
	Packets_t _localVar_3858;
	Packets_t _arg_3861;
	Packets_t _function_386a;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_3877;
		SFC::DT dT_3878;
		SFC::LocalVar localVar_3879;
		SFC::Arg arg_387a;
		SFC::Function function_387b;
	};

	std::list< Match> _matches;
};

class SaveStructState_388d
{
public:
	void operator()( const Packets_t& argdecls_388e, const Packets_t& dts_3890, const Packets_t& oldvals_3892, const Packets_t& contexts_3894, const Packets_t& functions_3896);

protected:
	void callSaveStructState_38e7( const Packets_t& argDeclBases_3899, const Packets_t& structs_389b, const Packets_t& localVars_389d, const Packets_t& contexts_389f, const Packets_t& functions_38a1);
};

class SaveStructState_3898
{
public:
	void operator()( const Packets_t& argDeclBases_3899, const Packets_t& structs_389b, const Packets_t& localVars_389d, const Packets_t& contexts_389f, const Packets_t& functions_38a1);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_38a7, const Udm::Object& struct_38b0, const Udm::Object& localVar_38b9, const Udm::Object& context_38c2, const Udm::Object& function_38cb);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_3899, const Packets_t& structs_389b, const Packets_t& localVars_389d, const Packets_t& contexts_389f, const Packets_t& functions_38a1);
	bool patternMatcher( const Udm::Object& argDeclBase_38a5, const Udm::Object& struct_38ae, const Udm::Object& localVar_38b7, const Udm::Object& context_38c0, const Udm::Object& function_38c9);
	void effector();

private:
	Packets_t _argDeclBase_38a3;
	Packets_t _struct_38ac;
	Packets_t _localVar_38b5;
	Packets_t _context_38be;
	Packets_t _function_38c7;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_38d7;
		SFC::Struct struct_38d8;
		SFC::LocalVar localVar_38d9;
		SFC::Arg context_38da;
		SFC::Function function_38db;
		SFC::LocalVar member_38dc;
	};

	std::list< Match> _matches;
};

class AddTriggerVars_38f9
{
public:
	void operator()( const Packets_t& systems_38fa, const Packets_t& childsystems_38fc, const Packets_t& argdecls_38fe, const Packets_t& dts_3900, const Packets_t& oldvals_3902, const Packets_t& contexts_3904, const Packets_t& functions_3906, Packets_t& systems_3908, Packets_t& childsystems_3909, Packets_t& argdecls_390a, Packets_t& dts_390b, Packets_t& oldvals_390c, Packets_t& contexts_390d, Packets_t& functions_390e);

protected:
	void callAddMatrixTriggerVars_4011( const Packets_t& childSubsystems_3cd6, const Packets_t& argdecls_3cd8, const Packets_t& dts_3cda, const Packets_t& oldvals_3cdc, const Packets_t& contexts_3cde, const Packets_t& functions_3ce0);
	void callAddStructTriggerVars_4018( const Packets_t& childSubsystems_3917, const Packets_t& argdecls_3919, const Packets_t& dts_391b, const Packets_t& oldvals_391d, const Packets_t& contexts_391f, const Packets_t& functions_3921);

private:
	Packets_t* _system_390f;
	Packets_t* _childsystem_3910;
	Packets_t* _argdecl_3911;
	Packets_t* _dt_3912;
	Packets_t* _oldval_3913;
	Packets_t* _context_3914;
	Packets_t* _function_3915;
};

class AddStructTriggerVars_3916
{
public:
	void operator()( const Packets_t& childSubsystems_3917, const Packets_t& argdecls_3919, const Packets_t& dts_391b, const Packets_t& oldvals_391d, const Packets_t& contexts_391f, const Packets_t& functions_3921);

protected:
	void callAddTriggerVar_3ca1( const Packets_t& charts_39b6, const Packets_t& argDeclBases_39b9, const Packets_t& structs_39bc, const Packets_t& localVars_39bf, const Packets_t& contexts_39c2, const Packets_t& functions_39c6);
	void callStartAssignment_3ca8( const Packets_t& charts_3925, const Packets_t& argDeclBases_3927, const Packets_t& structs_392b, const Packets_t& oldvals_392d, const Packets_t& contexts_3930, const Packets_t& triggerVars_3933, const Packets_t& functions_3935);
	void callEdgeTest_3cb0( const Packets_t& events_3a20, const Packets_t& argdecls_3a22, const Packets_t& members_3a24, const Packets_t& oldvals_3a26, const Packets_t& contexts_3a28, const Packets_t& initexprs_3a2a);
	void callRisingEdge_3cb7( const Packets_t& argDeclBases_3baa, const Packets_t& members_3bac, const Packets_t& localVars_3bae, const Packets_t& contexts_3bb0, const Packets_t& unaryExprss_3bb2);
	void callFallingEdge_3cbd( const Packets_t& argDeclBases_3bf8, const Packets_t& members_3bfa, const Packets_t& localVars_3bfc, const Packets_t& contexts_3bfe, const Packets_t& unaryExprss_3c00);
	void callEitherEdge_3cc3( const Packets_t& argDeclBases_3c46, const Packets_t& members_3c49, const Packets_t& localVars_3c4c, const Packets_t& contexts_3c4f, const Packets_t& unaryExprss_3c52);
	void callRisingEdge_3cc9( const Packets_t& argDeclBases_3baa, const Packets_t& members_3bac, const Packets_t& localVars_3bae, const Packets_t& contexts_3bb0, const Packets_t& unaryExprss_3bb2);
	void callFallingEdge_3ccf( const Packets_t& argDeclBases_3bf8, const Packets_t& members_3bfa, const Packets_t& localVars_3bfc, const Packets_t& contexts_3bfe, const Packets_t& unaryExprss_3c00);
};

class StartAssignment_3923
{
public:
	void operator()( const Packets_t& charts_3925, const Packets_t& argDeclBases_3927, const Packets_t& structs_392b, const Packets_t& oldvals_392d, const Packets_t& contexts_3930, const Packets_t& triggerVars_3933, const Packets_t& functions_3935, Packets_t& events_3924, Packets_t& argDeclBases_3929, Packets_t& localVars_392a, Packets_t& oldvals_392f, Packets_t& contexts_3932, Packets_t& unaryExprss_3937);

protected:
	bool isInputUnique( const Udm::Object& chart_3942, const Udm::Object& argDeclBase_394b, const Udm::Object& struct_3954, const Udm::Object& oldval_395d, const Udm::Object& context_3966, const Udm::Object& triggerVar_396f, const Udm::Object& function_3978);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, CyberComposition::Simulink::Subsystem& Chart, SFC::Arg& Context, CyberComposition::Simulink::Event& Event, SFC::Function& Function, SFC::LocalVar& LocalVar, CyberComposition::Simulink::Primitive& Primitive, CyberComposition::Simulink::State& State, SFC::Struct& Struct, SFC::LocalVar& TriggerVar, SFC::LocalVar& oldval);
	void processInputPackets( const Packets_t& charts_3925, const Packets_t& argDeclBases_3927, const Packets_t& structs_392b, const Packets_t& oldvals_392d, const Packets_t& contexts_3930, const Packets_t& triggerVars_3933, const Packets_t& functions_3935);
	bool patternMatcher( const Udm::Object& chart_3940, const Udm::Object& argDeclBase_3949, const Udm::Object& struct_3952, const Udm::Object& oldval_395b, const Udm::Object& context_3964, const Udm::Object& triggerVar_396d, const Udm::Object& function_3976);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Event& event_39a9, const SFC::ArgDeclBase& argDeclBase_39ab, const SFC::LocalVar& localVar_39ad, const SFC::LocalVar& oldval_39af, const SFC::Arg& context_39b1, const SFC::UnaryExprs& unaryExprs_39b3);

private:
	Packets_t* _event_3938;
	Packets_t* _argDeclBase_3939;
	Packets_t* _localVar_393a;
	Packets_t* _oldval_393b;
	Packets_t* _context_393c;
	Packets_t* _unaryExprs_393d;
	Packets_t _chart_393e;
	Packets_t _argDeclBase_3947;
	Packets_t _struct_3950;
	Packets_t _oldval_3959;
	Packets_t _context_3962;
	Packets_t _triggerVar_396b;
	Packets_t _function_3974;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem chart_398d;
		SFC::ArgDeclBase argDeclBase_398e;
		SFC::Struct struct_398f;
		SFC::LocalVar oldval_3990;
		SFC::Arg context_3991;
		SFC::LocalVar triggerVar_3992;
		SFC::Function function_3993;
		CyberComposition::Simulink::State state_3994;
		CyberComposition::Simulink::Event event_3995;
		CyberComposition::Simulink::Primitive primitive_3996;
		SFC::LocalVar localVar_3997;
	};

	std::list< Match> _matches;
};

class AddTriggerVar_39b5
{
public:
	void operator()( const Packets_t& charts_39b6, const Packets_t& argDeclBases_39b9, const Packets_t& structs_39bc, const Packets_t& localVars_39bf, const Packets_t& contexts_39c2, const Packets_t& functions_39c6, Packets_t& charts_39b8, Packets_t& argDeclBases_39bb, Packets_t& structs_39be, Packets_t& localVars_39c1, Packets_t& contexts_39c4, Packets_t& triggerVars_39c5, Packets_t& functions_39c8);

protected:
	bool isInputUnique( const Udm::Object& chart_39d4, const Udm::Object& argDeclBase_39dd, const Udm::Object& struct_39e6, const Udm::Object& localVar_39ef, const Udm::Object& context_39f8, const Udm::Object& function_3a01);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& charts_39b6, const Packets_t& argDeclBases_39b9, const Packets_t& structs_39bc, const Packets_t& localVars_39bf, const Packets_t& contexts_39c2, const Packets_t& functions_39c6);
	bool patternMatcher( const Udm::Object& chart_39d2, const Udm::Object& argDeclBase_39db, const Udm::Object& struct_39e4, const Udm::Object& localVar_39ed, const Udm::Object& context_39f6, const Udm::Object& function_39ff);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& chart_3a11, const SFC::ArgDeclBase& argDeclBase_3a13, const SFC::Struct& struct_3a15, const SFC::LocalVar& localVar_3a17, const SFC::Arg& context_3a19, const SFC::LocalVar& triggerVar_3a1b, const SFC::Function& function_3a1d);

private:
	Packets_t* _chart_39c9;
	Packets_t* _argDeclBase_39ca;
	Packets_t* _struct_39cb;
	Packets_t* _localVar_39cc;
	Packets_t* _context_39cd;
	Packets_t* _triggerVar_39ce;
	Packets_t* _function_39cf;
	Packets_t _chart_39d0;
	Packets_t _argDeclBase_39d9;
	Packets_t _struct_39e2;
	Packets_t _localVar_39eb;
	Packets_t _context_39f4;
	Packets_t _function_39fd;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem chart_3a0a;
		SFC::ArgDeclBase argDeclBase_3a0b;
		SFC::Struct struct_3a0c;
		SFC::LocalVar localVar_3a0d;
		SFC::Arg context_3a0e;
		SFC::Function function_3a0f;
	};

	std::list< Match> _matches;
};

class EdgeTest_3a1f
{
public:
	void operator()( const Packets_t& events_3a20, const Packets_t& argdecls_3a22, const Packets_t& members_3a24, const Packets_t& oldvals_3a26, const Packets_t& contexts_3a28, const Packets_t& initexprs_3a2a, Packets_t& argdecls_3a2c, Packets_t& members_3a2d, Packets_t& oldvals_3a2e, Packets_t& contexts_3a2f, Packets_t& initexprs_3a30, Packets_t& argdecls_3a31, Packets_t& members_3a32, Packets_t& oldvals_3a33, Packets_t& contexts_3a34, Packets_t& initexprs_3a35, Packets_t& argdecls_3a36, Packets_t& members_3a37, Packets_t& oldvals_3a38, Packets_t& contexts_3a39, Packets_t& initexprs_3a3a);

protected:
	void executeOne( const Packets_t& events_3a20, const Packets_t& argdecls_3a22, const Packets_t& members_3a24, const Packets_t& oldvals_3a26, const Packets_t& contexts_3a28, const Packets_t& initexprs_3a2a);
	bool isInputUnique( const Udm::Object& event_3a4c, const Udm::Object& argdecl_3a53, const Udm::Object& member_3a5a, const Udm::Object& oldval_3a61, const Udm::Object& context_3a68, const Udm::Object& initexpr_3a6f);

private:
	Packets_t* _argdecl_3a3b;
	Packets_t* _member_3a3c;
	Packets_t* _oldval_3a3d;
	Packets_t* _context_3a3e;
	Packets_t* _initexpr_3a3f;
	Packets_t* _argdecl_3a40;
	Packets_t* _member_3a41;
	Packets_t* _oldval_3a42;
	Packets_t* _context_3a43;
	Packets_t* _initexpr_3a44;
	Packets_t* _argdecl_3a45;
	Packets_t* _member_3a46;
	Packets_t* _oldval_3a47;
	Packets_t* _context_3a48;
	Packets_t* _initexpr_3a49;
	Packets_t _event_3a4a;
	Packets_t _argdecl_3a51;
	Packets_t _member_3a58;
	Packets_t _oldval_3a5f;
	Packets_t _context_3a66;
	Packets_t _initexpr_3a6d;
};

class RisingEdge_3a74
{
public:
	bool operator()( const Packets_t& events_3a75, const Packets_t& argDeclBases_3a77, const Packets_t& members_3a7a, const Packets_t& localVars_3a7d, const Packets_t& contexts_3a80, const Packets_t& unaryExprss_3a83, Packets_t& argDeclBases_3a79, Packets_t& members_3a7c, Packets_t& localVars_3a7f, Packets_t& contexts_3a82, Packets_t& unaryExprss_3a85);

protected:
	bool isInputUnique( const Udm::Object& event_3a8f, const Udm::Object& argDeclBase_3a98, const Udm::Object& member_3aa1, const Udm::Object& localVar_3aaa, const Udm::Object& context_3ab3, const Udm::Object& unaryExprs_3abc);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::Arg& Context, CyberComposition::Simulink::Event& Event, SFC::LocalVar& LocalVar, SFC::LocalVar& Member, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& events_3a75, const Packets_t& argDeclBases_3a77, const Packets_t& members_3a7a, const Packets_t& localVars_3a7d, const Packets_t& contexts_3a80, const Packets_t& unaryExprss_3a83);
	bool patternMatcher( const Udm::Object& event_3a8d, const Udm::Object& argDeclBase_3a96, const Udm::Object& member_3a9f, const Udm::Object& localVar_3aa8, const Udm::Object& context_3ab1, const Udm::Object& unaryExprs_3aba);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_3ad1, const SFC::LocalVar& member_3ad3, const SFC::LocalVar& localVar_3ad5, const SFC::Arg& context_3ad7, const SFC::UnaryExprs& unaryExprs_3ad9);

private:
	Packets_t* _argDeclBase_3a86;
	Packets_t* _member_3a87;
	Packets_t* _localVar_3a88;
	Packets_t* _context_3a89;
	Packets_t* _unaryExprs_3a8a;
	Packets_t _event_3a8b;
	Packets_t _argDeclBase_3a94;
	Packets_t _member_3a9d;
	Packets_t _localVar_3aa6;
	Packets_t _context_3aaf;
	Packets_t _unaryExprs_3ab8;
	class Match
	{
	public:
		CyberComposition::Simulink::Event event_3ac5;
		SFC::ArgDeclBase argDeclBase_3ac6;
		SFC::LocalVar member_3ac7;
		SFC::LocalVar localVar_3ac8;
		SFC::Arg context_3ac9;
		SFC::UnaryExprs unaryExprs_3aca;
	};

	std::list< Match> _matches;
};

class FallingEdge_3adb
{
public:
	bool operator()( const Packets_t& events_3adc, const Packets_t& argDeclBases_3ade, const Packets_t& members_3ae1, const Packets_t& localVars_3ae4, const Packets_t& contexts_3ae7, const Packets_t& unaryExprss_3aea, Packets_t& argDeclBases_3ae0, Packets_t& members_3ae3, Packets_t& localVars_3ae6, Packets_t& contexts_3ae9, Packets_t& unaryExprss_3aec);

protected:
	bool isInputUnique( const Udm::Object& event_3af6, const Udm::Object& argDeclBase_3aff, const Udm::Object& member_3b08, const Udm::Object& localVar_3b11, const Udm::Object& context_3b1a, const Udm::Object& unaryExprs_3b23);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::Arg& Context, CyberComposition::Simulink::Event& Event, SFC::LocalVar& LocalVar, SFC::LocalVar& Member, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& events_3adc, const Packets_t& argDeclBases_3ade, const Packets_t& members_3ae1, const Packets_t& localVars_3ae4, const Packets_t& contexts_3ae7, const Packets_t& unaryExprss_3aea);
	bool patternMatcher( const Udm::Object& event_3af4, const Udm::Object& argDeclBase_3afd, const Udm::Object& member_3b06, const Udm::Object& localVar_3b0f, const Udm::Object& context_3b18, const Udm::Object& unaryExprs_3b21);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_3b38, const SFC::LocalVar& member_3b3a, const SFC::LocalVar& localVar_3b3c, const SFC::Arg& context_3b3e, const SFC::UnaryExprs& unaryExprs_3b40);

private:
	Packets_t* _argDeclBase_3aed;
	Packets_t* _member_3aee;
	Packets_t* _localVar_3aef;
	Packets_t* _context_3af0;
	Packets_t* _unaryExprs_3af1;
	Packets_t _event_3af2;
	Packets_t _argDeclBase_3afb;
	Packets_t _member_3b04;
	Packets_t _localVar_3b0d;
	Packets_t _context_3b16;
	Packets_t _unaryExprs_3b1f;
	class Match
	{
	public:
		CyberComposition::Simulink::Event event_3b2c;
		SFC::ArgDeclBase argDeclBase_3b2d;
		SFC::LocalVar member_3b2e;
		SFC::LocalVar localVar_3b2f;
		SFC::Arg context_3b30;
		SFC::UnaryExprs unaryExprs_3b31;
	};

	std::list< Match> _matches;
};

class EitherEdge_3b42
{
public:
	bool operator()( const Packets_t& events_3b43, const Packets_t& argDeclBases_3b45, const Packets_t& members_3b48, const Packets_t& localVars_3b4b, const Packets_t& contexts_3b4e, const Packets_t& unaryExprss_3b51, Packets_t& argDeclBases_3b47, Packets_t& members_3b4a, Packets_t& localVars_3b4d, Packets_t& contexts_3b50, Packets_t& unaryExprss_3b53);

protected:
	bool isInputUnique( const Udm::Object& event_3b5d, const Udm::Object& argDeclBase_3b66, const Udm::Object& member_3b6f, const Udm::Object& localVar_3b78, const Udm::Object& context_3b81, const Udm::Object& unaryExprs_3b8a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::Arg& Context, CyberComposition::Simulink::Event& Event, SFC::LocalVar& LocalVar, SFC::LocalVar& Member, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& events_3b43, const Packets_t& argDeclBases_3b45, const Packets_t& members_3b48, const Packets_t& localVars_3b4b, const Packets_t& contexts_3b4e, const Packets_t& unaryExprss_3b51);
	bool patternMatcher( const Udm::Object& event_3b5b, const Udm::Object& argDeclBase_3b64, const Udm::Object& member_3b6d, const Udm::Object& localVar_3b76, const Udm::Object& context_3b7f, const Udm::Object& unaryExprs_3b88);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_3b9f, const SFC::LocalVar& member_3ba1, const SFC::LocalVar& localVar_3ba3, const SFC::Arg& context_3ba5, const SFC::UnaryExprs& unaryExprs_3ba7);

private:
	Packets_t* _argDeclBase_3b54;
	Packets_t* _member_3b55;
	Packets_t* _localVar_3b56;
	Packets_t* _context_3b57;
	Packets_t* _unaryExprs_3b58;
	Packets_t _event_3b59;
	Packets_t _argDeclBase_3b62;
	Packets_t _member_3b6b;
	Packets_t _localVar_3b74;
	Packets_t _context_3b7d;
	Packets_t _unaryExprs_3b86;
	class Match
	{
	public:
		CyberComposition::Simulink::Event event_3b93;
		SFC::ArgDeclBase argDeclBase_3b94;
		SFC::LocalVar member_3b95;
		SFC::LocalVar localVar_3b96;
		SFC::Arg context_3b97;
		SFC::UnaryExprs unaryExprs_3b98;
	};

	std::list< Match> _matches;
};

class RisingEdge_3ba9
{
public:
	void operator()( const Packets_t& argDeclBases_3baa, const Packets_t& members_3bac, const Packets_t& localVars_3bae, const Packets_t& contexts_3bb0, const Packets_t& unaryExprss_3bb2);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_3bb8, const Udm::Object& member_3bc1, const Udm::Object& localVar_3bca, const Udm::Object& context_3bd3, const Udm::Object& unaryExprs_3bdc);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_3baa, const Packets_t& members_3bac, const Packets_t& localVars_3bae, const Packets_t& contexts_3bb0, const Packets_t& unaryExprss_3bb2);
	bool patternMatcher( const Udm::Object& argDeclBase_3bb6, const Udm::Object& member_3bbf, const Udm::Object& localVar_3bc8, const Udm::Object& context_3bd1, const Udm::Object& unaryExprs_3bda);
	void effector();

private:
	Packets_t _argDeclBase_3bb4;
	Packets_t _member_3bbd;
	Packets_t _localVar_3bc6;
	Packets_t _context_3bcf;
	Packets_t _unaryExprs_3bd8;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_3be5;
		SFC::LocalVar member_3be6;
		SFC::LocalVar localVar_3be7;
		SFC::Arg context_3be8;
		SFC::UnaryExprs unaryExprs_3be9;
	};

	std::list< Match> _matches;
};

class FallingEdge_3bf7
{
public:
	void operator()( const Packets_t& argDeclBases_3bf8, const Packets_t& members_3bfa, const Packets_t& localVars_3bfc, const Packets_t& contexts_3bfe, const Packets_t& unaryExprss_3c00);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_3c06, const Udm::Object& member_3c0f, const Udm::Object& localVar_3c18, const Udm::Object& context_3c21, const Udm::Object& unaryExprs_3c2a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_3bf8, const Packets_t& members_3bfa, const Packets_t& localVars_3bfc, const Packets_t& contexts_3bfe, const Packets_t& unaryExprss_3c00);
	bool patternMatcher( const Udm::Object& argDeclBase_3c04, const Udm::Object& member_3c0d, const Udm::Object& localVar_3c16, const Udm::Object& context_3c1f, const Udm::Object& unaryExprs_3c28);
	void effector();

private:
	Packets_t _argDeclBase_3c02;
	Packets_t _member_3c0b;
	Packets_t _localVar_3c14;
	Packets_t _context_3c1d;
	Packets_t _unaryExprs_3c26;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_3c33;
		SFC::LocalVar member_3c34;
		SFC::LocalVar localVar_3c35;
		SFC::Arg context_3c36;
		SFC::UnaryExprs unaryExprs_3c37;
	};

	std::list< Match> _matches;
};

class EitherEdge_3c45
{
public:
	void operator()( const Packets_t& argDeclBases_3c46, const Packets_t& members_3c49, const Packets_t& localVars_3c4c, const Packets_t& contexts_3c4f, const Packets_t& unaryExprss_3c52, Packets_t& argDeclBases_3c48, Packets_t& members_3c4b, Packets_t& localVars_3c4e, Packets_t& contexts_3c51, Packets_t& leftUnaryExprss_3c54, Packets_t& rightUnaryExprss_3c55);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_3c60, const Udm::Object& member_3c69, const Udm::Object& localVar_3c72, const Udm::Object& context_3c7b, const Udm::Object& unaryExprs_3c84);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_3c46, const Packets_t& members_3c49, const Packets_t& localVars_3c4c, const Packets_t& contexts_3c4f, const Packets_t& unaryExprss_3c52);
	bool patternMatcher( const Udm::Object& argDeclBase_3c5e, const Udm::Object& member_3c67, const Udm::Object& localVar_3c70, const Udm::Object& context_3c79, const Udm::Object& unaryExprs_3c82);
	void effector();
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_3c95, const SFC::LocalVar& member_3c97, const SFC::LocalVar& localVar_3c99, const SFC::Arg& context_3c9b, const SFC::UnaryExprs& leftUnaryExprs_3c9d, const SFC::UnaryExprs& rightUnaryExprs_3c9f);

private:
	Packets_t* _argDeclBase_3c56;
	Packets_t* _member_3c57;
	Packets_t* _localVar_3c58;
	Packets_t* _context_3c59;
	Packets_t* _leftUnaryExprs_3c5a;
	Packets_t* _rightUnaryExprs_3c5b;
	Packets_t _argDeclBase_3c5c;
	Packets_t _member_3c65;
	Packets_t _localVar_3c6e;
	Packets_t _context_3c77;
	Packets_t _unaryExprs_3c80;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_3c8d;
		SFC::LocalVar member_3c8e;
		SFC::LocalVar localVar_3c8f;
		SFC::Arg context_3c90;
		SFC::UnaryExprs unaryExprs_3c91;
	};

	std::list< Match> _matches;
};

class AddMatrixTriggerVars_3cd5
{
public:
	void operator()( const Packets_t& childSubsystems_3cd6, const Packets_t& argdecls_3cd8, const Packets_t& dts_3cda, const Packets_t& oldvals_3cdc, const Packets_t& contexts_3cde, const Packets_t& functions_3ce0, Packets_t& childSubsystems_3ce2, Packets_t& argdecls_3ce3, Packets_t& dts_3ce4, Packets_t& oldvals_3ce5, Packets_t& contexts_3ce6, Packets_t& functions_3ce7);

protected:
	void callAddTriggerVar_3fe4( const Packets_t& charts_3cef, const Packets_t& argDeclBases_3cf2, const Packets_t& dTs_3cf5, const Packets_t& localVars_3cf7, const Packets_t& contexts_3cfa, const Packets_t& functions_3cfe);
	void callStartAssignment_3feb( const Packets_t& charts_3d5c, const Packets_t& argDeclBases_3d5e, const Packets_t& oldvals_3d61, const Packets_t& contexts_3d64, const Packets_t& triggerVars_3d67, const Packets_t& functions_3d69);
	void callEdgeTest_3ff2( const Packets_t& events_3dd5, const Packets_t& argdecls_3dd7, const Packets_t& oldvals_3dd9, const Packets_t& contexts_3ddb, const Packets_t& initexprs_3ddd);
	void callRisingEdge_3ff8( const Packets_t& argDeclBases_3f1d, const Packets_t& localVars_3f1f, const Packets_t& contexts_3f21, const Packets_t& unaryExprss_3f23);
	void callFallingEdge_3ffd( const Packets_t& argDeclBases_3f5b, const Packets_t& localVars_3f5d, const Packets_t& contexts_3f5f, const Packets_t& unaryExprss_3f61);
	void callEitherEdge_4002( const Packets_t& argDeclBases_3f99, const Packets_t& localVars_3f9c, const Packets_t& contexts_3f9f, const Packets_t& unaryExprss_3fa2);
	void callRisingEdge_4007( const Packets_t& argDeclBases_3f1d, const Packets_t& localVars_3f1f, const Packets_t& contexts_3f21, const Packets_t& unaryExprss_3f23);
	void callFallingEdge_400c( const Packets_t& argDeclBases_3f5b, const Packets_t& localVars_3f5d, const Packets_t& contexts_3f5f, const Packets_t& unaryExprss_3f61);

private:
	Packets_t* _childSubsystem_3ce8;
	Packets_t* _argdecl_3ce9;
	Packets_t* _dt_3cea;
	Packets_t* _oldval_3ceb;
	Packets_t* _context_3cec;
	Packets_t* _function_3ced;
};

class AddTriggerVar_3cee
{
public:
	void operator()( const Packets_t& charts_3cef, const Packets_t& argDeclBases_3cf2, const Packets_t& dTs_3cf5, const Packets_t& localVars_3cf7, const Packets_t& contexts_3cfa, const Packets_t& functions_3cfe, Packets_t& charts_3cf1, Packets_t& argDeclBases_3cf4, Packets_t& localVars_3cf9, Packets_t& contexts_3cfc, Packets_t& triggerVars_3cfd, Packets_t& functions_3d00);

protected:
	bool isInputUnique( const Udm::Object& chart_3d0b, const Udm::Object& argDeclBase_3d14, const Udm::Object& dT_3d1d, const Udm::Object& localVar_3d26, const Udm::Object& context_3d2f, const Udm::Object& function_3d38);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, CyberComposition::Simulink::Subsystem& Chart, SFC::Arg& Context, SFC::DT& DT, SFC::Function& Function, SFC::LocalVar& LocalVar);
	void processInputPackets( const Packets_t& charts_3cef, const Packets_t& argDeclBases_3cf2, const Packets_t& dTs_3cf5, const Packets_t& localVars_3cf7, const Packets_t& contexts_3cfa, const Packets_t& functions_3cfe);
	bool patternMatcher( const Udm::Object& chart_3d09, const Udm::Object& argDeclBase_3d12, const Udm::Object& dT_3d1b, const Udm::Object& localVar_3d24, const Udm::Object& context_3d2d, const Udm::Object& function_3d36);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& chart_3d4e, const SFC::ArgDeclBase& argDeclBase_3d50, const SFC::LocalVar& localVar_3d52, const SFC::Arg& context_3d54, const SFC::LocalVar& triggerVar_3d56, const SFC::Function& function_3d58);

private:
	Packets_t* _chart_3d01;
	Packets_t* _argDeclBase_3d02;
	Packets_t* _localVar_3d03;
	Packets_t* _context_3d04;
	Packets_t* _triggerVar_3d05;
	Packets_t* _function_3d06;
	Packets_t _chart_3d07;
	Packets_t _argDeclBase_3d10;
	Packets_t _dT_3d19;
	Packets_t _localVar_3d22;
	Packets_t _context_3d2b;
	Packets_t _function_3d34;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem chart_3d41;
		SFC::ArgDeclBase argDeclBase_3d42;
		SFC::DT dT_3d43;
		SFC::LocalVar localVar_3d44;
		SFC::Arg context_3d45;
		SFC::Function function_3d46;
	};

	std::list< Match> _matches;
};

class StartAssignment_3d5a
{
public:
	void operator()( const Packets_t& charts_3d5c, const Packets_t& argDeclBases_3d5e, const Packets_t& oldvals_3d61, const Packets_t& contexts_3d64, const Packets_t& triggerVars_3d67, const Packets_t& functions_3d69, Packets_t& events_3d5b, Packets_t& argDeclBases_3d60, Packets_t& oldvals_3d63, Packets_t& contexts_3d66, Packets_t& unaryExprss_3d6b);

protected:
	bool isInputUnique( const Udm::Object& chart_3d75, const Udm::Object& argDeclBase_3d7e, const Udm::Object& oldval_3d87, const Udm::Object& context_3d90, const Udm::Object& triggerVar_3d99, const Udm::Object& function_3da2);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, CyberComposition::Simulink::Subsystem& Chart, SFC::Arg& Context, CyberComposition::Simulink::Event& Event, SFC::Function& Function, CyberComposition::Simulink::Primitive& Primitive, CyberComposition::Simulink::State& State, SFC::LocalVar& TriggerVar, SFC::LocalVar& oldval);
	void processInputPackets( const Packets_t& charts_3d5c, const Packets_t& argDeclBases_3d5e, const Packets_t& oldvals_3d61, const Packets_t& contexts_3d64, const Packets_t& triggerVars_3d67, const Packets_t& functions_3d69);
	bool patternMatcher( const Udm::Object& chart_3d73, const Udm::Object& argDeclBase_3d7c, const Udm::Object& oldval_3d85, const Udm::Object& context_3d8e, const Udm::Object& triggerVar_3d97, const Udm::Object& function_3da0);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Event& event_3dca, const SFC::ArgDeclBase& argDeclBase_3dcc, const SFC::LocalVar& oldval_3dce, const SFC::Arg& context_3dd0, const SFC::UnaryExprs& unaryExprs_3dd2);

private:
	Packets_t* _event_3d6c;
	Packets_t* _argDeclBase_3d6d;
	Packets_t* _oldval_3d6e;
	Packets_t* _context_3d6f;
	Packets_t* _unaryExprs_3d70;
	Packets_t _chart_3d71;
	Packets_t _argDeclBase_3d7a;
	Packets_t _oldval_3d83;
	Packets_t _context_3d8c;
	Packets_t _triggerVar_3d95;
	Packets_t _function_3d9e;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem chart_3db4;
		SFC::ArgDeclBase argDeclBase_3db5;
		SFC::LocalVar oldval_3db6;
		SFC::Arg context_3db7;
		SFC::LocalVar triggerVar_3db8;
		SFC::Function function_3db9;
		CyberComposition::Simulink::State state_3dba;
		CyberComposition::Simulink::Event event_3dbb;
		CyberComposition::Simulink::Primitive primitive_3dbc;
	};

	std::list< Match> _matches;
};

class EdgeTest_3dd4
{
public:
	void operator()( const Packets_t& events_3dd5, const Packets_t& argdecls_3dd7, const Packets_t& oldvals_3dd9, const Packets_t& contexts_3ddb, const Packets_t& initexprs_3ddd, Packets_t& argdecls_3ddf, Packets_t& oldvals_3de0, Packets_t& contexts_3de1, Packets_t& initexprs_3de2, Packets_t& argdecls_3de3, Packets_t& oldvals_3de4, Packets_t& contexts_3de5, Packets_t& initexprs_3de6, Packets_t& argdecls_3de7, Packets_t& oldvals_3de8, Packets_t& contexts_3de9, Packets_t& initexprs_3dea);

protected:
	void executeOne( const Packets_t& events_3dd5, const Packets_t& argdecls_3dd7, const Packets_t& oldvals_3dd9, const Packets_t& contexts_3ddb, const Packets_t& initexprs_3ddd);
	bool isInputUnique( const Udm::Object& event_3df9, const Udm::Object& argdecl_3e00, const Udm::Object& oldval_3e07, const Udm::Object& context_3e0e, const Udm::Object& initexpr_3e15);

private:
	Packets_t* _argdecl_3deb;
	Packets_t* _oldval_3dec;
	Packets_t* _context_3ded;
	Packets_t* _initexpr_3dee;
	Packets_t* _argdecl_3def;
	Packets_t* _oldval_3df0;
	Packets_t* _context_3df1;
	Packets_t* _initexpr_3df2;
	Packets_t* _argdecl_3df3;
	Packets_t* _oldval_3df4;
	Packets_t* _context_3df5;
	Packets_t* _initexpr_3df6;
	Packets_t _event_3df7;
	Packets_t _argdecl_3dfe;
	Packets_t _oldval_3e05;
	Packets_t _context_3e0c;
	Packets_t _initexpr_3e13;
};

class RisingEdge_3e1a
{
public:
	bool operator()( const Packets_t& events_3e1b, const Packets_t& argDeclBases_3e1d, const Packets_t& localVars_3e20, const Packets_t& contexts_3e23, const Packets_t& unaryExprss_3e26, Packets_t& argDeclBases_3e1f, Packets_t& localVars_3e22, Packets_t& contexts_3e25, Packets_t& unaryExprss_3e28);

protected:
	bool isInputUnique( const Udm::Object& event_3e31, const Udm::Object& argDeclBase_3e3a, const Udm::Object& localVar_3e43, const Udm::Object& context_3e4c, const Udm::Object& unaryExprs_3e55);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::Arg& Context, CyberComposition::Simulink::Event& Event, SFC::LocalVar& LocalVar, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& events_3e1b, const Packets_t& argDeclBases_3e1d, const Packets_t& localVars_3e20, const Packets_t& contexts_3e23, const Packets_t& unaryExprss_3e26);
	bool patternMatcher( const Udm::Object& event_3e2f, const Udm::Object& argDeclBase_3e38, const Udm::Object& localVar_3e41, const Udm::Object& context_3e4a, const Udm::Object& unaryExprs_3e53);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_3e68, const SFC::LocalVar& localVar_3e6a, const SFC::Arg& context_3e6c, const SFC::UnaryExprs& unaryExprs_3e6e);

private:
	Packets_t* _argDeclBase_3e29;
	Packets_t* _localVar_3e2a;
	Packets_t* _context_3e2b;
	Packets_t* _unaryExprs_3e2c;
	Packets_t _event_3e2d;
	Packets_t _argDeclBase_3e36;
	Packets_t _localVar_3e3f;
	Packets_t _context_3e48;
	Packets_t _unaryExprs_3e51;
	class Match
	{
	public:
		CyberComposition::Simulink::Event event_3e5e;
		SFC::ArgDeclBase argDeclBase_3e5f;
		SFC::LocalVar localVar_3e60;
		SFC::Arg context_3e61;
		SFC::UnaryExprs unaryExprs_3e62;
	};

	std::list< Match> _matches;
};

class FallingEdge_3e70
{
public:
	bool operator()( const Packets_t& events_3e71, const Packets_t& argDeclBases_3e73, const Packets_t& localVars_3e76, const Packets_t& contexts_3e79, const Packets_t& unaryExprss_3e7c, Packets_t& argDeclBases_3e75, Packets_t& localVars_3e78, Packets_t& contexts_3e7b, Packets_t& unaryExprss_3e7e);

protected:
	bool isInputUnique( const Udm::Object& event_3e87, const Udm::Object& argDeclBase_3e90, const Udm::Object& localVar_3e99, const Udm::Object& context_3ea2, const Udm::Object& unaryExprs_3eab);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::Arg& Context, CyberComposition::Simulink::Event& Event, SFC::LocalVar& LocalVar, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& events_3e71, const Packets_t& argDeclBases_3e73, const Packets_t& localVars_3e76, const Packets_t& contexts_3e79, const Packets_t& unaryExprss_3e7c);
	bool patternMatcher( const Udm::Object& event_3e85, const Udm::Object& argDeclBase_3e8e, const Udm::Object& localVar_3e97, const Udm::Object& context_3ea0, const Udm::Object& unaryExprs_3ea9);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_3ebe, const SFC::LocalVar& localVar_3ec0, const SFC::Arg& context_3ec2, const SFC::UnaryExprs& unaryExprs_3ec4);

private:
	Packets_t* _argDeclBase_3e7f;
	Packets_t* _localVar_3e80;
	Packets_t* _context_3e81;
	Packets_t* _unaryExprs_3e82;
	Packets_t _event_3e83;
	Packets_t _argDeclBase_3e8c;
	Packets_t _localVar_3e95;
	Packets_t _context_3e9e;
	Packets_t _unaryExprs_3ea7;
	class Match
	{
	public:
		CyberComposition::Simulink::Event event_3eb4;
		SFC::ArgDeclBase argDeclBase_3eb5;
		SFC::LocalVar localVar_3eb6;
		SFC::Arg context_3eb7;
		SFC::UnaryExprs unaryExprs_3eb8;
	};

	std::list< Match> _matches;
};

class EitherEdge_3ec6
{
public:
	bool operator()( const Packets_t& events_3ec7, const Packets_t& argDeclBases_3ec9, const Packets_t& localVars_3ecc, const Packets_t& contexts_3ecf, const Packets_t& unaryExprss_3ed2, Packets_t& argDeclBases_3ecb, Packets_t& localVars_3ece, Packets_t& contexts_3ed1, Packets_t& unaryExprss_3ed4);

protected:
	bool isInputUnique( const Udm::Object& event_3edd, const Udm::Object& argDeclBase_3ee6, const Udm::Object& localVar_3eef, const Udm::Object& context_3ef8, const Udm::Object& unaryExprs_3f01);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::Arg& Context, CyberComposition::Simulink::Event& Event, SFC::LocalVar& LocalVar, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& events_3ec7, const Packets_t& argDeclBases_3ec9, const Packets_t& localVars_3ecc, const Packets_t& contexts_3ecf, const Packets_t& unaryExprss_3ed2);
	bool patternMatcher( const Udm::Object& event_3edb, const Udm::Object& argDeclBase_3ee4, const Udm::Object& localVar_3eed, const Udm::Object& context_3ef6, const Udm::Object& unaryExprs_3eff);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_3f14, const SFC::LocalVar& localVar_3f16, const SFC::Arg& context_3f18, const SFC::UnaryExprs& unaryExprs_3f1a);

private:
	Packets_t* _argDeclBase_3ed5;
	Packets_t* _localVar_3ed6;
	Packets_t* _context_3ed7;
	Packets_t* _unaryExprs_3ed8;
	Packets_t _event_3ed9;
	Packets_t _argDeclBase_3ee2;
	Packets_t _localVar_3eeb;
	Packets_t _context_3ef4;
	Packets_t _unaryExprs_3efd;
	class Match
	{
	public:
		CyberComposition::Simulink::Event event_3f0a;
		SFC::ArgDeclBase argDeclBase_3f0b;
		SFC::LocalVar localVar_3f0c;
		SFC::Arg context_3f0d;
		SFC::UnaryExprs unaryExprs_3f0e;
	};

	std::list< Match> _matches;
};

class RisingEdge_3f1c
{
public:
	void operator()( const Packets_t& argDeclBases_3f1d, const Packets_t& localVars_3f1f, const Packets_t& contexts_3f21, const Packets_t& unaryExprss_3f23);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_3f29, const Udm::Object& localVar_3f32, const Udm::Object& context_3f3b, const Udm::Object& unaryExprs_3f44);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_3f1d, const Packets_t& localVars_3f1f, const Packets_t& contexts_3f21, const Packets_t& unaryExprss_3f23);
	bool patternMatcher( const Udm::Object& argDeclBase_3f27, const Udm::Object& localVar_3f30, const Udm::Object& context_3f39, const Udm::Object& unaryExprs_3f42);
	void effector();

private:
	Packets_t _argDeclBase_3f25;
	Packets_t _localVar_3f2e;
	Packets_t _context_3f37;
	Packets_t _unaryExprs_3f40;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_3f4d;
		SFC::LocalVar localVar_3f4e;
		SFC::Arg context_3f4f;
		SFC::UnaryExprs unaryExprs_3f50;
	};

	std::list< Match> _matches;
};

class FallingEdge_3f5a
{
public:
	void operator()( const Packets_t& argDeclBases_3f5b, const Packets_t& localVars_3f5d, const Packets_t& contexts_3f5f, const Packets_t& unaryExprss_3f61);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_3f67, const Udm::Object& localVar_3f70, const Udm::Object& context_3f79, const Udm::Object& unaryExprs_3f82);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_3f5b, const Packets_t& localVars_3f5d, const Packets_t& contexts_3f5f, const Packets_t& unaryExprss_3f61);
	bool patternMatcher( const Udm::Object& argDeclBase_3f65, const Udm::Object& localVar_3f6e, const Udm::Object& context_3f77, const Udm::Object& unaryExprs_3f80);
	void effector();

private:
	Packets_t _argDeclBase_3f63;
	Packets_t _localVar_3f6c;
	Packets_t _context_3f75;
	Packets_t _unaryExprs_3f7e;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_3f8b;
		SFC::LocalVar localVar_3f8c;
		SFC::Arg context_3f8d;
		SFC::UnaryExprs unaryExprs_3f8e;
	};

	std::list< Match> _matches;
};

class EitherEdge_3f98
{
public:
	void operator()( const Packets_t& argDeclBases_3f99, const Packets_t& localVars_3f9c, const Packets_t& contexts_3f9f, const Packets_t& unaryExprss_3fa2, Packets_t& argDeclBases_3f9b, Packets_t& localVars_3f9e, Packets_t& contexts_3fa1, Packets_t& leftUnaryExprss_3fa4, Packets_t& rightUnaryExprss_3fa5);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_3faf, const Udm::Object& localVar_3fb8, const Udm::Object& context_3fc1, const Udm::Object& unaryExprs_3fca);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_3f99, const Packets_t& localVars_3f9c, const Packets_t& contexts_3f9f, const Packets_t& unaryExprss_3fa2);
	bool patternMatcher( const Udm::Object& argDeclBase_3fad, const Udm::Object& localVar_3fb6, const Udm::Object& context_3fbf, const Udm::Object& unaryExprs_3fc8);
	void effector();
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_3fda, const SFC::LocalVar& localVar_3fdc, const SFC::Arg& context_3fde, const SFC::UnaryExprs& leftUnaryExprs_3fe0, const SFC::UnaryExprs& rightUnaryExprs_3fe2);

private:
	Packets_t* _argDeclBase_3fa6;
	Packets_t* _localVar_3fa7;
	Packets_t* _context_3fa8;
	Packets_t* _leftUnaryExprs_3fa9;
	Packets_t* _rightUnaryExprs_3faa;
	Packets_t _argDeclBase_3fab;
	Packets_t _localVar_3fb4;
	Packets_t _context_3fbd;
	Packets_t _unaryExprs_3fc6;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_3fd3;
		SFC::LocalVar localVar_3fd4;
		SFC::Arg context_3fd5;
		SFC::UnaryExprs unaryExprs_3fd6;
	};

	std::list< Match> _matches;
};

class GetTriggerPort_403b
{
public:
	void operator()( const Packets_t& subsystems_403c, const Packets_t& blocks_403f, const Packets_t& mains_4042, Packets_t& subsystems_403e, Packets_t& triggerPorts_4041, Packets_t& mains_4044);

protected:
	bool isInputUnique( const Udm::Object& subsystem_404c, const Udm::Object& block_4055, const Udm::Object& main_405e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_403c, const Packets_t& blocks_403f, const Packets_t& mains_4042);
	bool patternMatcher( const Udm::Object& subsystem_404a, const Udm::Object& block_4053, const Udm::Object& main_405c);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_406e, const CyberComposition::Simulink::TriggerPort& triggerPort_4070, const SFC::Function& main_4072);

private:
	Packets_t* _subsystem_4045;
	Packets_t* _triggerPort_4046;
	Packets_t* _main_4047;
	Packets_t _subsystem_4048;
	Packets_t _block_4051;
	Packets_t _main_405a;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_406a;
		CyberComposition::Simulink::Block block_406b;
		SFC::Function main_406c;
		CyberComposition::Simulink::TriggerPort triggerPort_406d;
	};

	std::list< Match> _matches;
};

class AssignCondValLocalVar_4074
{
public:
	void operator()( const Packets_t& subsystems_4075, const Packets_t& triggerPorts_4078, const Packets_t& functions_407d, Packets_t& subsystems_4077, Packets_t& triggerPorts_407a, Packets_t& argDeclBases_407b, Packets_t& dTs_407c, Packets_t& functions_407f);

protected:
	bool isInputUnique( const Udm::Object& subsystem_4089, const Udm::Object& triggerPort_4092, const Udm::Object& function_409b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_4075, const Packets_t& triggerPorts_4078, const Packets_t& functions_407d);
	bool patternMatcher( const Udm::Object& subsystem_4087, const Udm::Object& triggerPort_4090, const Udm::Object& function_4099);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_40ab, const CyberComposition::Simulink::SF_Port& triggerPort_40ad, const SFC::ArgDeclBase& argDeclBase_40af, const SFC::DT& dT_40b1, const SFC::Function& function_40b3);

private:
	Packets_t* _subsystem_4080;
	Packets_t* _triggerPort_4081;
	Packets_t* _argDeclBase_4082;
	Packets_t* _dT_4083;
	Packets_t* _function_4084;
	Packets_t _subsystem_4085;
	Packets_t _triggerPort_408e;
	Packets_t _function_4097;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_40a6;
		CyberComposition::Simulink::SF_Port triggerPort_40a7;
		SFC::Function function_40a8;
		SFC::ArgDeclBase argDeclBase_40a9;
		SFC::DT dT_40aa;
	};

	std::list< Match> _matches;
};

class IsTriggered_40c3
{
public:
	void operator()( const Packets_t& systems_40c4, const Packets_t& childSubsystems_40c6, const Packets_t& systemFunctions_40c8, Packets_t& systems_40ca, Packets_t& childSubsystems_40cb, Packets_t& systemFunctions_40cc, Packets_t& systems_40cd, Packets_t& childSubsystems_40ce, Packets_t& systemFunctions_40cf);

protected:
	void executeOne( const Packets_t& systems_40c4, const Packets_t& childSubsystems_40c6, const Packets_t& systemFunctions_40c8);
	bool isInputUnique( const Udm::Object& system_40d8, const Udm::Object& childSubsystem_40df, const Udm::Object& systemFunction_40e6);

private:
	Packets_t* _system_40d0;
	Packets_t* _childSubsystem_40d1;
	Packets_t* _systemFunction_40d2;
	Packets_t* _system_40d3;
	Packets_t* _childSubsystem_40d4;
	Packets_t* _systemFunction_40d5;
	Packets_t _system_40d6;
	Packets_t _childSubsystem_40dd;
	Packets_t _systemFunction_40e4;
};

class HasTriggers_40eb
{
public:
	bool operator()( const Packets_t& subsystems_40ec, const Packets_t& blocks_40ef, const Packets_t& mains_40f2, Packets_t& subsystems_40ee, Packets_t& blocks_40f1, Packets_t& mains_40f4);

protected:
	bool isInputUnique( const Udm::Object& subsystem_40fc, const Udm::Object& block_4105, const Udm::Object& main_410e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_40ec, const Packets_t& blocks_40ef, const Packets_t& mains_40f2);
	bool patternMatcher( const Udm::Object& subsystem_40fa, const Udm::Object& block_4103, const Udm::Object& main_410c);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_411e, const CyberComposition::Simulink::Subsystem& block_4120, const SFC::Function& main_4122);

private:
	Packets_t* _subsystem_40f5;
	Packets_t* _block_40f6;
	Packets_t* _main_40f7;
	Packets_t _subsystem_40f8;
	Packets_t _block_4101;
	Packets_t _main_410a;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_411a;
		CyberComposition::Simulink::Subsystem block_411b;
		SFC::Function main_411c;
		CyberComposition::Simulink::TriggerPort triggerPort_411d;
	};

	std::list< Match> _matches;
};

class Otherwise_4124
{
public:
	bool operator()( const Packets_t& subsystems_4125, const Packets_t& blocks_4128, const Packets_t& mains_412b, Packets_t& subsystems_4127, Packets_t& blocks_412a, Packets_t& mains_412d);

protected:
	bool isInputUnique( const Udm::Object& subsystem_4135, const Udm::Object& block_413e, const Udm::Object& main_4147);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_4125, const Packets_t& blocks_4128, const Packets_t& mains_412b);
	bool patternMatcher( const Udm::Object& subsystem_4133, const Udm::Object& block_413c, const Udm::Object& main_4145);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_4153, const CyberComposition::Simulink::Subsystem& block_4155, const SFC::Function& main_4157);

private:
	Packets_t* _subsystem_412e;
	Packets_t* _block_412f;
	Packets_t* _main_4130;
	Packets_t _subsystem_4131;
	Packets_t _block_413a;
	Packets_t _main_4143;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_4150;
		CyberComposition::Simulink::Subsystem block_4151;
		SFC::Function main_4152;
	};

	std::list< Match> _matches;
};

class SFChartChildBlock_4186
{
public:
	void operator()( const Packets_t& systems_4187, const Packets_t& childCharts_4189, const Packets_t& systemFunctions_418b);

protected:
	void callMakeTrigger_5287( const Packets_t& systems_453c, const Packets_t& childSubsystems_453e, const Packets_t& systemFunctions_4540);
	void callCreateFunctionCall_528b( const Packets_t& subsystems_418e, const Packets_t& charts_4192, const Packets_t& compoundStatements_4196);
	void callGetState_528f( const Packets_t& subsystems_523b, const Packets_t& charts_523e, const Packets_t& mains_5242, const Packets_t& functionCalls_5245);
	void callInputData_5294( const Packets_t& systems_42b6, const Packets_t& childCharts_42b8, const Packets_t& states_42ba, const Packets_t& functions_42bc, const Packets_t& functionCalls_42be);
	void callInputEvent_529a( const Packets_t& systems_4414, const Packets_t& childCharts_4416, const Packets_t& states_4418, const Packets_t& functions_441a, const Packets_t& functionCalls_441c);
	void callOutputData_52a0( const Packets_t& systems_41d8, const Packets_t& childCharts_41da, const Packets_t& states_41dc, const Packets_t& functions_41de, const Packets_t& functionCalls_41e0);
	void callOutputEvent_52a6( const Packets_t& systems_4394, const Packets_t& childCharts_4396, const Packets_t& states_4398, const Packets_t& functions_439a, const Packets_t& functionCalls_439c);
};

class CreateFunctionCall_418d
{
public:
	void operator()( const Packets_t& subsystems_418e, const Packets_t& charts_4192, const Packets_t& compoundStatements_4196, Packets_t& subsystems_4190, Packets_t& mains_4191, Packets_t& charts_4194, Packets_t& functionCalls_4195);

protected:
	bool isInputUnique( const Udm::Object& subsystem_41a0, const Udm::Object& chart_41a9, const Udm::Object& compoundStatement_41b2);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_418e, const Packets_t& charts_4192, const Packets_t& compoundStatements_4196);
	bool patternMatcher( const Udm::Object& subsystem_419e, const Udm::Object& chart_41a7, const Udm::Object& compoundStatement_41b0);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_41cf, const SFC::Function& main_41d1, const CyberComposition::Simulink::Subsystem& chart_41d3, const SFC::FunctionCall& functionCall_41d5);

private:
	Packets_t* _subsystem_4198;
	Packets_t* _main_4199;
	Packets_t* _chart_419a;
	Packets_t* _functionCall_419b;
	Packets_t _subsystem_419c;
	Packets_t _chart_41a5;
	Packets_t _compoundStatement_41ae;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_41c6;
		CyberComposition::Simulink::Subsystem chart_41c7;
		SFC::CompoundStatement compoundStatement_41c8;
		SFC::Class class_41c9;
		SFC::Struct struct_41ca;
		SFC::Arg arg_41cb;
		SFC::Function main_41cc;
	};

	std::list< Match> _matches;
};

class OutputData_41d7
{
public:
	void operator()( const Packets_t& systems_41d8, const Packets_t& childCharts_41da, const Packets_t& states_41dc, const Packets_t& functions_41de, const Packets_t& functionCalls_41e0, Packets_t& systems_41e2, Packets_t& childCharts_41e3, Packets_t& states_41e4, Packets_t& functions_41e5, Packets_t& functionCalls_41e6);

protected:
	void callCreateOutputDataArgVal_42a5( const Packets_t& subsystems_4241, const Packets_t& charts_4245, const Packets_t& states_4247, const Packets_t& mains_4249, const Packets_t& functionCalls_424d);
	void callUpdateArgCount_42ab( const Packets_t& subsystems_41ed, const Packets_t& outputPorts_41f0, const Packets_t& functions_41f3, const Packets_t& argVals_41f5, const Packets_t& functionCalls_41f8);
	void callMakeArgDeclRef_42b1( const Packets_t& systems_2cf6, const Packets_t& argPorts_2cf8, const Packets_t& argVals_2cfa);

private:
	Packets_t* _system_41e7;
	Packets_t* _childChart_41e8;
	Packets_t* _state_41e9;
	Packets_t* _function_41ea;
	Packets_t* _functionCall_41eb;
};

class UpdateArgCount_41ec
{
public:
	void operator()( const Packets_t& subsystems_41ed, const Packets_t& outputPorts_41f0, const Packets_t& functions_41f3, const Packets_t& argVals_41f5, const Packets_t& functionCalls_41f8, Packets_t& subsystems_41ef, Packets_t& outputPorts_41f2, Packets_t& argVals_41f7);

protected:
	bool isInputUnique( const Udm::Object& subsystem_4201, const Udm::Object& outputPort_420a, const Udm::Object& function_4213, const Udm::Object& argVal_421c, const Udm::Object& functionCall_4225);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg, SFC::ArgVal& ArgVal, SFC::Function& Function, SFC::FunctionCall& FunctionCall, CyberComposition::Simulink::OutputPort& OutputPort, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& subsystems_41ed, const Packets_t& outputPorts_41f0, const Packets_t& functions_41f3, const Packets_t& argVals_41f5, const Packets_t& functionCalls_41f8);
	bool patternMatcher( const Udm::Object& subsystem_41ff, const Udm::Object& outputPort_4208, const Udm::Object& function_4211, const Udm::Object& argVal_421a, const Udm::Object& functionCall_4223);
	void effector();
	void forwardInputs();

private:
	Packets_t* _subsystem_41fa;
	Packets_t* _outputPort_41fb;
	Packets_t* _argVal_41fc;
	Packets_t _subsystem_41fd;
	Packets_t _outputPort_4206;
	Packets_t _function_420f;
	Packets_t _argVal_4218;
	Packets_t _functionCall_4221;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_4234;
		CyberComposition::Simulink::OutputPort outputPort_4235;
		SFC::Function function_4236;
		SFC::ArgVal argVal_4237;
		SFC::FunctionCall functionCall_4238;
		SFC::Arg arg_4239;
	};

	std::list< Match> _matches;
};

class CreateOutputDataArgVal_4240
{
public:
	void operator()( const Packets_t& subsystems_4241, const Packets_t& charts_4245, const Packets_t& states_4247, const Packets_t& mains_4249, const Packets_t& functionCalls_424d, Packets_t& subsystems_4243, Packets_t& outputPorts_4244, Packets_t& mains_424b, Packets_t& argVals_424c, Packets_t& functionCalls_424f);

protected:
	bool isInputUnique( const Udm::Object& subsystem_4259, const Udm::Object& chart_4262, const Udm::Object& state_426b, const Udm::Object& main_4274, const Udm::Object& functionCall_427d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::Subsystem& Chart, CyberComposition::Simulink::SFData& Data, SFC::FunctionCall& FunctionCall, SFC::Function& Main, CyberComposition::Simulink::OutputPort& OutputPort, CyberComposition::Simulink::SFState& State, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& subsystems_4241, const Packets_t& charts_4245, const Packets_t& states_4247, const Packets_t& mains_4249, const Packets_t& functionCalls_424d);
	bool patternMatcher( const Udm::Object& subsystem_4257, const Udm::Object& chart_4260, const Udm::Object& state_4269, const Udm::Object& main_4272, const Udm::Object& functionCall_427b);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_429b, const CyberComposition::Simulink::OutputPort& outputPort_429d, const SFC::Function& main_429f, const SFC::ArgVal& argVal_42a1, const SFC::FunctionCall& functionCall_42a3);

private:
	Packets_t* _subsystem_4250;
	Packets_t* _outputPort_4251;
	Packets_t* _main_4252;
	Packets_t* _argVal_4253;
	Packets_t* _functionCall_4254;
	Packets_t _subsystem_4255;
	Packets_t _chart_425e;
	Packets_t _state_4267;
	Packets_t _main_4270;
	Packets_t _functionCall_4279;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_428c;
		CyberComposition::Simulink::Subsystem chart_428d;
		CyberComposition::Simulink::SFState state_428e;
		SFC::Function main_428f;
		SFC::FunctionCall functionCall_4290;
		CyberComposition::Simulink::OutputPort outputPort_4291;
		CyberComposition::Simulink::SFData data_4292;
	};

	std::list< Match> _matches;
};

class InputData_42b5
{
public:
	void operator()( const Packets_t& systems_42b6, const Packets_t& childCharts_42b8, const Packets_t& states_42ba, const Packets_t& functions_42bc, const Packets_t& functionCalls_42be, Packets_t& systems_42c0, Packets_t& childCharts_42c1, Packets_t& states_42c2, Packets_t& functions_42c3, Packets_t& functionCalls_42c4);

protected:
	void callCreateInputDataArgVal_4383( const Packets_t& subsystems_431f, const Packets_t& charts_4323, const Packets_t& states_4325, const Packets_t& mains_4327, const Packets_t& functionCalls_432b);
	void callUpdateArgCount_4389( const Packets_t& subsystems_42cb, const Packets_t& inputPorts_42ce, const Packets_t& functions_42d1, const Packets_t& argVals_42d3, const Packets_t& functionCalls_42d6);
	void callMakeArgDeclRef_438f( const Packets_t& systems_2cf6, const Packets_t& argPorts_2cf8, const Packets_t& argVals_2cfa);

private:
	Packets_t* _system_42c5;
	Packets_t* _childChart_42c6;
	Packets_t* _state_42c7;
	Packets_t* _function_42c8;
	Packets_t* _functionCall_42c9;
};

class UpdateArgCount_42ca
{
public:
	void operator()( const Packets_t& subsystems_42cb, const Packets_t& inputPorts_42ce, const Packets_t& functions_42d1, const Packets_t& argVals_42d3, const Packets_t& functionCalls_42d6, Packets_t& subsystems_42cd, Packets_t& inputPorts_42d0, Packets_t& argVals_42d5);

protected:
	bool isInputUnique( const Udm::Object& subsystem_42df, const Udm::Object& inputPort_42e8, const Udm::Object& function_42f1, const Udm::Object& argVal_42fa, const Udm::Object& functionCall_4303);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg, SFC::ArgVal& ArgVal, SFC::Function& Function, SFC::FunctionCall& FunctionCall, CyberComposition::Simulink::InputPort& InputPort, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& subsystems_42cb, const Packets_t& inputPorts_42ce, const Packets_t& functions_42d1, const Packets_t& argVals_42d3, const Packets_t& functionCalls_42d6);
	bool patternMatcher( const Udm::Object& subsystem_42dd, const Udm::Object& inputPort_42e6, const Udm::Object& function_42ef, const Udm::Object& argVal_42f8, const Udm::Object& functionCall_4301);
	void effector();
	void forwardInputs();

private:
	Packets_t* _subsystem_42d8;
	Packets_t* _inputPort_42d9;
	Packets_t* _argVal_42da;
	Packets_t _subsystem_42db;
	Packets_t _inputPort_42e4;
	Packets_t _function_42ed;
	Packets_t _argVal_42f6;
	Packets_t _functionCall_42ff;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_4312;
		CyberComposition::Simulink::InputPort inputPort_4313;
		SFC::Function function_4314;
		SFC::ArgVal argVal_4315;
		SFC::FunctionCall functionCall_4316;
		SFC::Arg arg_4317;
	};

	std::list< Match> _matches;
};

class CreateInputDataArgVal_431e
{
public:
	void operator()( const Packets_t& subsystems_431f, const Packets_t& charts_4323, const Packets_t& states_4325, const Packets_t& mains_4327, const Packets_t& functionCalls_432b, Packets_t& subsystems_4321, Packets_t& inputPorts_4322, Packets_t& mains_4329, Packets_t& argVals_432a, Packets_t& functionCalls_432d);

protected:
	bool isInputUnique( const Udm::Object& subsystem_4337, const Udm::Object& chart_4340, const Udm::Object& state_4349, const Udm::Object& main_4352, const Udm::Object& functionCall_435b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::Subsystem& Chart, CyberComposition::Simulink::SFData& Data, SFC::FunctionCall& FunctionCall, CyberComposition::Simulink::InputPort& InputPort, SFC::Function& Main, CyberComposition::Simulink::SFState& State, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& subsystems_431f, const Packets_t& charts_4323, const Packets_t& states_4325, const Packets_t& mains_4327, const Packets_t& functionCalls_432b);
	bool patternMatcher( const Udm::Object& subsystem_4335, const Udm::Object& chart_433e, const Udm::Object& state_4347, const Udm::Object& main_4350, const Udm::Object& functionCall_4359);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_4379, const CyberComposition::Simulink::InputPort& inputPort_437b, const SFC::Function& main_437d, const SFC::ArgVal& argVal_437f, const SFC::FunctionCall& functionCall_4381);

private:
	Packets_t* _subsystem_432e;
	Packets_t* _inputPort_432f;
	Packets_t* _main_4330;
	Packets_t* _argVal_4331;
	Packets_t* _functionCall_4332;
	Packets_t _subsystem_4333;
	Packets_t _chart_433c;
	Packets_t _state_4345;
	Packets_t _main_434e;
	Packets_t _functionCall_4357;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_436a;
		CyberComposition::Simulink::Subsystem chart_436b;
		CyberComposition::Simulink::SFState state_436c;
		SFC::Function main_436d;
		SFC::FunctionCall functionCall_436e;
		CyberComposition::Simulink::InputPort inputPort_436f;
		CyberComposition::Simulink::SFData data_4370;
	};

	std::list< Match> _matches;
};

class OutputEvent_4393
{
public:
	void operator()( const Packets_t& systems_4394, const Packets_t& childCharts_4396, const Packets_t& states_4398, const Packets_t& functions_439a, const Packets_t& functionCalls_439c);

protected:
	void callCreateOutputEventArgVal_4403( const Packets_t& subsystems_439f, const Packets_t& charts_43a3, const Packets_t& states_43a5, const Packets_t& mains_43a7, const Packets_t& functionCalls_43ab);
	void callUpdateArgCount_4409( const Packets_t& subsystems_41ed, const Packets_t& outputPorts_41f0, const Packets_t& functions_41f3, const Packets_t& argVals_41f5, const Packets_t& functionCalls_41f8);
	void callMakeArgDeclRef_440f( const Packets_t& systems_2cf6, const Packets_t& argPorts_2cf8, const Packets_t& argVals_2cfa);
};

class CreateOutputEventArgVal_439e
{
public:
	void operator()( const Packets_t& subsystems_439f, const Packets_t& charts_43a3, const Packets_t& states_43a5, const Packets_t& mains_43a7, const Packets_t& functionCalls_43ab, Packets_t& subsystems_43a1, Packets_t& outputPorts_43a2, Packets_t& mains_43a9, Packets_t& argVals_43aa, Packets_t& functionCalls_43ad);

protected:
	bool isInputUnique( const Udm::Object& subsystem_43b7, const Udm::Object& chart_43c0, const Udm::Object& state_43c9, const Udm::Object& main_43d2, const Udm::Object& functionCall_43db);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::Subsystem& Chart, CyberComposition::Simulink::SFEvent& Event, SFC::FunctionCall& FunctionCall, SFC::Function& Main, CyberComposition::Simulink::OutputPort& OutputPort, CyberComposition::Simulink::SFState& State, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& subsystems_439f, const Packets_t& charts_43a3, const Packets_t& states_43a5, const Packets_t& mains_43a7, const Packets_t& functionCalls_43ab);
	bool patternMatcher( const Udm::Object& subsystem_43b5, const Udm::Object& chart_43be, const Udm::Object& state_43c7, const Udm::Object& main_43d0, const Udm::Object& functionCall_43d9);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_43f9, const CyberComposition::Simulink::OutputPort& outputPort_43fb, const SFC::Function& main_43fd, const SFC::ArgVal& argVal_43ff, const SFC::FunctionCall& functionCall_4401);

private:
	Packets_t* _subsystem_43ae;
	Packets_t* _outputPort_43af;
	Packets_t* _main_43b0;
	Packets_t* _argVal_43b1;
	Packets_t* _functionCall_43b2;
	Packets_t _subsystem_43b3;
	Packets_t _chart_43bc;
	Packets_t _state_43c5;
	Packets_t _main_43ce;
	Packets_t _functionCall_43d7;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_43ea;
		CyberComposition::Simulink::Subsystem chart_43eb;
		CyberComposition::Simulink::SFState state_43ec;
		SFC::Function main_43ed;
		SFC::FunctionCall functionCall_43ee;
		CyberComposition::Simulink::OutputPort outputPort_43ef;
		CyberComposition::Simulink::SFEvent event_43f0;
	};

	std::list< Match> _matches;
};

class InputEvent_4413
{
public:
	void operator()( const Packets_t& systems_4414, const Packets_t& childCharts_4416, const Packets_t& states_4418, const Packets_t& functions_441a, const Packets_t& functionCalls_441c, Packets_t& systems_441e, Packets_t& childCharts_441f, Packets_t& states_4420, Packets_t& functions_4421, Packets_t& functionCalls_4422);

protected:
	void callGetOldVar_452f( const Packets_t& blocks_44f6, const Packets_t& mains_44f8, const Packets_t& functionCalls_44fb);
	void callMatrixInputEvent_4533( const Packets_t& oldvars_44ab, const Packets_t& functions_44ad, const Packets_t& functionCalls_44af);
	void callStructInputEvent_4537( const Packets_t& oldvars_4429, const Packets_t& functions_442b, const Packets_t& functionCalls_442d);

private:
	Packets_t* _system_4423;
	Packets_t* _childChart_4424;
	Packets_t* _state_4425;
	Packets_t* _function_4426;
	Packets_t* _functionCall_4427;
};

class StructInputEvent_4428
{
public:
	void operator()( const Packets_t& oldvars_4429, const Packets_t& functions_442b, const Packets_t& functionCalls_442d);

protected:
	void callStructInputEventArgVal_44a2( const Packets_t& localVars_4461, const Packets_t& mains_4463, const Packets_t& functionCalls_4467);
	void callUpdateArgCount_44a6( const Packets_t& functions_4430, const Packets_t& argVals_4432, const Packets_t& functionCalls_4434);
};

class UpdateArgCount_442f
{
public:
	void operator()( const Packets_t& functions_4430, const Packets_t& argVals_4432, const Packets_t& functionCalls_4434);

protected:
	bool isInputUnique( const Udm::Object& function_443a, const Udm::Object& argVal_4443, const Udm::Object& functionCall_444c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg, SFC::ArgVal& ArgVal, SFC::Function& Function, SFC::FunctionCall& FunctionCall);
	void processInputPackets( const Packets_t& functions_4430, const Packets_t& argVals_4432, const Packets_t& functionCalls_4434);
	bool patternMatcher( const Udm::Object& function_4438, const Udm::Object& argVal_4441, const Udm::Object& functionCall_444a);
	void effector();

private:
	Packets_t _function_4436;
	Packets_t _argVal_443f;
	Packets_t _functionCall_4448;
	class Match
	{
	public:
		SFC::Function function_4458;
		SFC::ArgVal argVal_4459;
		SFC::FunctionCall functionCall_445a;
		SFC::Arg arg_445b;
	};

	std::list< Match> _matches;
};

class StructInputEventArgVal_4460
{
public:
	void operator()( const Packets_t& localVars_4461, const Packets_t& mains_4463, const Packets_t& functionCalls_4467, Packets_t& mains_4465, Packets_t& argVals_4466, Packets_t& functionCalls_4469);

protected:
	bool isInputUnique( const Udm::Object& localVar_4471, const Udm::Object& main_447a, const Udm::Object& functionCall_4483);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& localVars_4461, const Packets_t& mains_4463, const Packets_t& functionCalls_4467);
	bool patternMatcher( const Udm::Object& localVar_446f, const Udm::Object& main_4478, const Udm::Object& functionCall_4481);
	void effector();
	void outputAppender( const SFC::Function& main_449c, const SFC::ArgVal& argVal_449e, const SFC::FunctionCall& functionCall_44a0);

private:
	Packets_t* _main_446a;
	Packets_t* _argVal_446b;
	Packets_t* _functionCall_446c;
	Packets_t _localVar_446d;
	Packets_t _main_4476;
	Packets_t _functionCall_447f;
	class Match
	{
	public:
		SFC::LocalVar localVar_4492;
		SFC::Function main_4493;
		SFC::FunctionCall functionCall_4494;
		SFC::LocalVar triggerVar_4495;
		SFC::Struct struct_4496;
		SFC::LocalVar member_4497;
	};

	std::list< Match> _matches;
};

class MatrixInputEvent_44aa
{
public:
	void operator()( const Packets_t& oldvars_44ab, const Packets_t& functions_44ad, const Packets_t& functionCalls_44af, Packets_t& oldvars_44b1, Packets_t& functions_44b2, Packets_t& functionCalls_44b3);

protected:
	void callMatrixInputEventArgVal_44f0( const Packets_t& localVars_44b8, const Packets_t& functions_44ba, const Packets_t& functionCalls_44bc);

private:
	Packets_t* _oldvar_44b4;
	Packets_t* _function_44b5;
	Packets_t* _functionCall_44b6;
};

class MatrixInputEventArgVal_44b7
{
public:
	void operator()( const Packets_t& localVars_44b8, const Packets_t& functions_44ba, const Packets_t& functionCalls_44bc);

protected:
	bool isInputUnique( const Udm::Object& localVar_44c2, const Udm::Object& function_44cb, const Udm::Object& functionCall_44d4);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg, SFC::DT& DT, SFC::Function& Function, SFC::FunctionCall& FunctionCall, SFC::LocalVar& LocalVar, SFC::LocalVar& TriggerVar);
	void processInputPackets( const Packets_t& localVars_44b8, const Packets_t& functions_44ba, const Packets_t& functionCalls_44bc);
	bool patternMatcher( const Udm::Object& localVar_44c0, const Udm::Object& function_44c9, const Udm::Object& functionCall_44d2);
	void effector();

private:
	Packets_t _localVar_44be;
	Packets_t _function_44c7;
	Packets_t _functionCall_44d0;
	class Match
	{
	public:
		SFC::LocalVar localVar_44e2;
		SFC::Function function_44e3;
		SFC::FunctionCall functionCall_44e4;
		SFC::DT dT_44e5;
		SFC::LocalVar triggerVar_44e6;
		SFC::Arg arg_44e7;
	};

	std::list< Match> _matches;
};

class GetOldVar_44f4
{
public:
	void operator()( const Packets_t& blocks_44f6, const Packets_t& mains_44f8, const Packets_t& functionCalls_44fb, Packets_t& localVars_44f5, Packets_t& mains_44fa, Packets_t& functionCalls_44fd);

protected:
	bool isInputUnique( const Udm::Object& block_4505, const Udm::Object& main_450e, const Udm::Object& functionCall_4517);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& blocks_44f6, const Packets_t& mains_44f8, const Packets_t& functionCalls_44fb);
	bool patternMatcher( const Udm::Object& block_4503, const Udm::Object& main_450c, const Udm::Object& functionCall_4515);
	void effector();
	void outputAppender( const SFC::LocalVar& localVar_4529, const SFC::Function& main_452b, const SFC::FunctionCall& functionCall_452d);

private:
	Packets_t* _localVar_44fe;
	Packets_t* _main_44ff;
	Packets_t* _functionCall_4500;
	Packets_t _block_4501;
	Packets_t _main_450a;
	Packets_t _functionCall_4513;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem block_4524;
		SFC::Function main_4525;
		SFC::FunctionCall functionCall_4526;
		SFC::LocalVar localVar_4527;
		CyberComposition::Simulink::TriggerPort triggerPort_4528;
	};

	std::list< Match> _matches;
};

class MakeTrigger_453b
{
public:
	void operator()( const Packets_t& systems_453c, const Packets_t& childSubsystems_453e, const Packets_t& systemFunctions_4540, Packets_t& systems_4542, Packets_t& childSubsystems_4543, Packets_t& compoundStatements_4544);

protected:
	void callIsTriggered_5232( const Packets_t& systems_4549, const Packets_t& childSubsystems_454b, const Packets_t& systemFunctions_454d);
	void callCreateTriggering_5236( const Packets_t& systems_45df, const Packets_t& childSubsystems_45e1, const Packets_t& systemFunctions_45e3);

private:
	Packets_t* _system_4545;
	Packets_t* _childSubsystem_4546;
	Packets_t* _compoundStatement_4547;
};

class IsTriggered_4548
{
public:
	void operator()( const Packets_t& systems_4549, const Packets_t& childSubsystems_454b, const Packets_t& systemFunctions_454d, Packets_t& systems_454f, Packets_t& childSubsystems_4550, Packets_t& systemFunctions_4551, Packets_t& systems_4552, Packets_t& childSubsystems_4553, Packets_t& systemFunctions_4554);

protected:
	void executeOne( const Packets_t& systems_4549, const Packets_t& childSubsystems_454b, const Packets_t& systemFunctions_454d);
	bool isInputUnique( const Udm::Object& system_455d, const Udm::Object& childSubsystem_4564, const Udm::Object& systemFunction_456b);

private:
	Packets_t* _system_4555;
	Packets_t* _childSubsystem_4556;
	Packets_t* _systemFunction_4557;
	Packets_t* _system_4558;
	Packets_t* _childSubsystem_4559;
	Packets_t* _systemFunction_455a;
	Packets_t _system_455b;
	Packets_t _childSubsystem_4562;
	Packets_t _systemFunction_4569;
};

class HasTriggers_4570
{
public:
	bool operator()( const Packets_t& subsystems_4571, const Packets_t& blocks_4574, const Packets_t& mains_4577, Packets_t& subsystems_4573, Packets_t& blocks_4576, Packets_t& mains_4579);

protected:
	bool isInputUnique( const Udm::Object& subsystem_4581, const Udm::Object& block_458a, const Udm::Object& main_4593);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_4571, const Packets_t& blocks_4574, const Packets_t& mains_4577);
	bool patternMatcher( const Udm::Object& subsystem_457f, const Udm::Object& block_4588, const Udm::Object& main_4591);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_45a3, const CyberComposition::Simulink::Subsystem& block_45a5, const SFC::Function& main_45a7);

private:
	Packets_t* _subsystem_457a;
	Packets_t* _block_457b;
	Packets_t* _main_457c;
	Packets_t _subsystem_457d;
	Packets_t _block_4586;
	Packets_t _main_458f;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_459f;
		CyberComposition::Simulink::Subsystem block_45a0;
		SFC::Function main_45a1;
		CyberComposition::Simulink::TriggerPort triggerPort_45a2;
	};

	std::list< Match> _matches;
};

class Otherwise_45a9
{
public:
	bool operator()( const Packets_t& subsystems_45aa, const Packets_t& blocks_45ad, const Packets_t& mains_45b0, Packets_t& subsystems_45ac, Packets_t& blocks_45af, Packets_t& mains_45b2);

protected:
	bool isInputUnique( const Udm::Object& subsystem_45ba, const Udm::Object& block_45c3, const Udm::Object& main_45cc);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_45aa, const Packets_t& blocks_45ad, const Packets_t& mains_45b0);
	bool patternMatcher( const Udm::Object& subsystem_45b8, const Udm::Object& block_45c1, const Udm::Object& main_45ca);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_45d8, const CyberComposition::Simulink::Subsystem& block_45da, const SFC::Function& main_45dc);

private:
	Packets_t* _subsystem_45b3;
	Packets_t* _block_45b4;
	Packets_t* _main_45b5;
	Packets_t _subsystem_45b6;
	Packets_t _block_45bf;
	Packets_t _main_45c8;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_45d5;
		CyberComposition::Simulink::Subsystem block_45d6;
		SFC::Function main_45d7;
	};

	std::list< Match> _matches;
};

class CreateTriggering_45de
{
public:
	void operator()( const Packets_t& systems_45df, const Packets_t& childSubsystems_45e1, const Packets_t& systemFunctions_45e3, Packets_t& systems_45e5, Packets_t& childSubsystems_45e6, Packets_t& conditionalBlocks_45e7);

protected:
	void callGetTriggerPort_5224( const Packets_t& subsystems_45ec, const Packets_t& blocks_45ef, const Packets_t& mains_45f2);
	void callAssignCondValLocalVar_5228( const Packets_t& subsystems_4625, const Packets_t& triggerPorts_4628, const Packets_t& functions_462d);
	void callMakeTrigger_522c( const Packets_t& systems_4666, const Packets_t& childSubsystemTriggers_4668, const Packets_t& argdecls_466a, const Packets_t& dts_466c, const Packets_t& functions_466e);

private:
	Packets_t* _system_45e8;
	Packets_t* _childSubsystem_45e9;
	Packets_t* _conditionalBlock_45ea;
};

class GetTriggerPort_45eb
{
public:
	void operator()( const Packets_t& subsystems_45ec, const Packets_t& blocks_45ef, const Packets_t& mains_45f2, Packets_t& subsystems_45ee, Packets_t& triggerPorts_45f1, Packets_t& mains_45f4);

protected:
	bool isInputUnique( const Udm::Object& subsystem_45fc, const Udm::Object& block_4605, const Udm::Object& main_460e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_45ec, const Packets_t& blocks_45ef, const Packets_t& mains_45f2);
	bool patternMatcher( const Udm::Object& subsystem_45fa, const Udm::Object& block_4603, const Udm::Object& main_460c);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_461e, const CyberComposition::Simulink::TriggerPort& triggerPort_4620, const SFC::Function& main_4622);

private:
	Packets_t* _subsystem_45f5;
	Packets_t* _triggerPort_45f6;
	Packets_t* _main_45f7;
	Packets_t _subsystem_45f8;
	Packets_t _block_4601;
	Packets_t _main_460a;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_461a;
		CyberComposition::Simulink::Block block_461b;
		SFC::Function main_461c;
		CyberComposition::Simulink::TriggerPort triggerPort_461d;
	};

	std::list< Match> _matches;
};

class AssignCondValLocalVar_4624
{
public:
	void operator()( const Packets_t& subsystems_4625, const Packets_t& triggerPorts_4628, const Packets_t& functions_462d, Packets_t& subsystems_4627, Packets_t& triggerPorts_462a, Packets_t& argDeclBases_462b, Packets_t& dTs_462c, Packets_t& functions_462f);

protected:
	bool isInputUnique( const Udm::Object& subsystem_4639, const Udm::Object& triggerPort_4642, const Udm::Object& function_464b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_4625, const Packets_t& triggerPorts_4628, const Packets_t& functions_462d);
	bool patternMatcher( const Udm::Object& subsystem_4637, const Udm::Object& triggerPort_4640, const Udm::Object& function_4649);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_465b, const CyberComposition::Simulink::SF_Port& triggerPort_465d, const SFC::ArgDeclBase& argDeclBase_465f, const SFC::DT& dT_4661, const SFC::Function& function_4663);

private:
	Packets_t* _subsystem_4630;
	Packets_t* _triggerPort_4631;
	Packets_t* _argDeclBase_4632;
	Packets_t* _dT_4633;
	Packets_t* _function_4634;
	Packets_t _subsystem_4635;
	Packets_t _triggerPort_463e;
	Packets_t _function_4647;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_4656;
		CyberComposition::Simulink::SF_Port triggerPort_4657;
		SFC::Function function_4658;
		SFC::ArgDeclBase argDeclBase_4659;
		SFC::DT dT_465a;
	};

	std::list< Match> _matches;
};

class MakeTrigger_4665
{
public:
	void operator()( const Packets_t& systems_4666, const Packets_t& childSubsystemTriggers_4668, const Packets_t& argdecls_466a, const Packets_t& dts_466c, const Packets_t& functions_466e, Packets_t& systems_4670, Packets_t& childSubsystems_4671, Packets_t& conditionals_4672);

protected:
	void callAddStateVar_5208( const Packets_t& subsystems_4989, const Packets_t& triggerPorts_498c, const Packets_t& argDeclBases_498f, const Packets_t& dTs_4993, const Packets_t& functions_4997);
	void callAddTriggerVars_520e( const Packets_t& systems_4ae3, const Packets_t& childsystems_4ae5, const Packets_t& argdecls_4ae7, const Packets_t& dts_4ae9, const Packets_t& oldvals_4aeb, const Packets_t& contexts_4aed, const Packets_t& functions_4aef);
	void callSaveState_5216( const Packets_t& systems_49f7, const Packets_t& childsystems_49f9, const Packets_t& argdecls_49fb, const Packets_t& dts_49fd, const Packets_t& oldvals_49ff, const Packets_t& contexts_4a01, const Packets_t& functions_4a03);
	void callMakeCondition_521e( const Packets_t& systems_4677, const Packets_t& childSubsystems_4679, const Packets_t& dts_467b, const Packets_t& oldvals_467d, const Packets_t& functions_467f);

private:
	Packets_t* _system_4673;
	Packets_t* _childSubsystem_4674;
	Packets_t* _conditional_4675;
};

class MakeCondition_4676
{
public:
	void operator()( const Packets_t& systems_4677, const Packets_t& childSubsystems_4679, const Packets_t& dts_467b, const Packets_t& oldvals_467d, const Packets_t& functions_467f, Packets_t& systems_4681, Packets_t& childSubsystems_4682, Packets_t& conditionals_4683);

protected:
	void callCreateConditional_497a( const Packets_t& subsystems_491e, const Packets_t& charts_4921, const Packets_t& dTs_4924, const Packets_t& localVars_4927, const Packets_t& functions_492b);
	void callSetMatrixCondition_4980( const Packets_t& dts_48e0, const Packets_t& triggervars_48e2, const Packets_t& conditions_48e4);
	void callSetStructCondition_4984( const Packets_t& dts_4688, const Packets_t& oldvals_468a, const Packets_t& conditions_468c);

private:
	Packets_t* _system_4684;
	Packets_t* _childSubsystem_4685;
	Packets_t* _conditional_4686;
};

class SetStructCondition_4687
{
public:
	void operator()( const Packets_t& dts_4688, const Packets_t& oldvals_468a, const Packets_t& conditions_468c);

protected:
	void callStartCondition_48d2( const Packets_t& structs_468f, const Packets_t& triggerVars_4692, const Packets_t& userCodes_4695);
	void callgetStructMembers_48d6( const Packets_t& structs_4896, const Packets_t& triggerVars_489a, const Packets_t& unaryExprss_489d);
	void callPlaceMember_48da( const Packets_t& structs_46c5, const Packets_t& members_46c7, const Packets_t& trigvars_46c9, const Packets_t& condexprs_46cb);
};

class StartCondition_468e
{
public:
	void operator()( const Packets_t& structs_468f, const Packets_t& triggerVars_4692, const Packets_t& userCodes_4695, Packets_t& structs_4691, Packets_t& triggerVars_4694, Packets_t& unaryExprss_4697);

protected:
	bool isInputUnique( const Udm::Object& struct_469f, const Udm::Object& triggerVar_46a8, const Udm::Object& userCode_46b1);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& structs_468f, const Packets_t& triggerVars_4692, const Packets_t& userCodes_4695);
	bool patternMatcher( const Udm::Object& struct_469d, const Udm::Object& triggerVar_46a6, const Udm::Object& userCode_46af);
	void effector();
	void outputAppender( const SFC::Struct& struct_46be, const SFC::LocalVar& triggerVar_46c0, const SFC::UnaryExprs& unaryExprs_46c2);

private:
	Packets_t* _struct_4698;
	Packets_t* _triggerVar_4699;
	Packets_t* _unaryExprs_469a;
	Packets_t _struct_469b;
	Packets_t _triggerVar_46a4;
	Packets_t _userCode_46ad;
	class Match
	{
	public:
		SFC::Struct struct_46ba;
		SFC::LocalVar triggerVar_46bb;
		SFC::UserCode userCode_46bc;
	};

	std::list< Match> _matches;
};

class PlaceMember_46c4
{
public:
	void operator()( const Packets_t& structs_46c5, const Packets_t& members_46c7, const Packets_t& trigvars_46c9, const Packets_t& condexprs_46cb);

protected:
	void executeOne( const Packets_t& structs_46c5, const Packets_t& members_46c7, const Packets_t& trigvars_46c9, const Packets_t& condexprs_46cb);
	bool isInputUnique( const Udm::Object& struct_46cf, const Udm::Object& member_46d6, const Udm::Object& trigvar_46dd, const Udm::Object& condexpr_46e4);
	void callTriggerTest_4886( const Packets_t& structs_4777, const Packets_t& members_4779, const Packets_t& trigvars_477b, const Packets_t& condexprs_477d);
	void callAddTrigger_488b( const Packets_t& structs_46ea, const Packets_t& members_46ec, const Packets_t& trigvars_46ee, const Packets_t& condexprs_46f0);
	void callTraverseBinary_4890( const Packets_t& structs_483a, const Packets_t& members_483d, const Packets_t& triggerVars_4840, const Packets_t& unaryExprss_4843);

private:
	Packets_t _struct_46cd;
	Packets_t _member_46d4;
	Packets_t _trigvar_46db;
	Packets_t _condexpr_46e2;
};

class AddTrigger_46e9
{
public:
	void operator()( const Packets_t& structs_46ea, const Packets_t& members_46ec, const Packets_t& trigvars_46ee, const Packets_t& condexprs_46f0);

protected:
	void callAddLastTrigger_476c( const Packets_t& structs_46f3, const Packets_t& members_46f5, const Packets_t& triggerVars_46f7, const Packets_t& unaryExprss_46f9);
	void callAddNextTrigger_4771( const Packets_t& structs_472f, const Packets_t& members_4731, const Packets_t& triggerVars_4733, const Packets_t& unaryExprss_4735);
};

class AddLastTrigger_46f2
{
public:
	void operator()( const Packets_t& structs_46f3, const Packets_t& members_46f5, const Packets_t& triggerVars_46f7, const Packets_t& unaryExprss_46f9);

protected:
	bool isInputUnique( const Udm::Object& struct_46ff, const Udm::Object& member_4708, const Udm::Object& triggerVar_4711, const Udm::Object& unaryExprs_471a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::LocalVar& Member, SFC::Struct& Struct, SFC::LocalVar& TriggerVar, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& structs_46f3, const Packets_t& members_46f5, const Packets_t& triggerVars_46f7, const Packets_t& unaryExprss_46f9);
	bool patternMatcher( const Udm::Object& struct_46fd, const Udm::Object& member_4706, const Udm::Object& triggerVar_470f, const Udm::Object& unaryExprs_4718);
	void effector();

private:
	Packets_t _struct_46fb;
	Packets_t _member_4704;
	Packets_t _triggerVar_470d;
	Packets_t _unaryExprs_4716;
	class Match
	{
	public:
		SFC::Struct struct_4723;
		SFC::LocalVar member_4724;
		SFC::LocalVar triggerVar_4725;
		SFC::UnaryExprs unaryExprs_4726;
	};

	std::list< Match> _matches;
};

class AddNextTrigger_472e
{
public:
	void operator()( const Packets_t& structs_472f, const Packets_t& members_4731, const Packets_t& triggerVars_4733, const Packets_t& unaryExprss_4735);

protected:
	bool isInputUnique( const Udm::Object& struct_473b, const Udm::Object& member_4744, const Udm::Object& triggerVar_474d, const Udm::Object& unaryExprs_4756);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::LocalVar& Member, SFC::Struct& Struct, SFC::LocalVar& TriggerVar, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& structs_472f, const Packets_t& members_4731, const Packets_t& triggerVars_4733, const Packets_t& unaryExprss_4735);
	bool patternMatcher( const Udm::Object& struct_4739, const Udm::Object& member_4742, const Udm::Object& triggerVar_474b, const Udm::Object& unaryExprs_4754);
	void effector();

private:
	Packets_t _struct_4737;
	Packets_t _member_4740;
	Packets_t _triggerVar_4749;
	Packets_t _unaryExprs_4752;
	class Match
	{
	public:
		SFC::Struct struct_475f;
		SFC::LocalVar member_4760;
		SFC::LocalVar triggerVar_4761;
		SFC::UnaryExprs unaryExprs_4762;
	};

	std::list< Match> _matches;
};

class TriggerTest_4776
{
public:
	void operator()( const Packets_t& structs_4777, const Packets_t& members_4779, const Packets_t& trigvars_477b, const Packets_t& condexprs_477d, Packets_t& structs_477f, Packets_t& members_4780, Packets_t& trigvars_4781, Packets_t& condexprs_4782, Packets_t& structs_4783, Packets_t& members_4784, Packets_t& trigvars_4785, Packets_t& condexprs_4786);

protected:
	void executeOne( const Packets_t& structs_4777, const Packets_t& members_4779, const Packets_t& trigvars_477b, const Packets_t& condexprs_477d);
	bool isInputUnique( const Udm::Object& struct_4791, const Udm::Object& member_4798, const Udm::Object& trigvar_479f, const Udm::Object& condexpr_47a6);

private:
	Packets_t* _struct_4787;
	Packets_t* _member_4788;
	Packets_t* _trigvar_4789;
	Packets_t* _condexpr_478a;
	Packets_t* _struct_478b;
	Packets_t* _member_478c;
	Packets_t* _trigvar_478d;
	Packets_t* _condexpr_478e;
	Packets_t _struct_478f;
	Packets_t _member_4796;
	Packets_t _trigvar_479d;
	Packets_t _condexpr_47a4;
};

class HasBinaryExprs_47ab
{
public:
	bool operator()( const Packets_t& structs_47ac, const Packets_t& members_47af, const Packets_t& triggerVars_47b2, const Packets_t& unaryExprss_47b5, Packets_t& structs_47ae, Packets_t& members_47b1, Packets_t& triggerVars_47b4, Packets_t& unaryExprss_47b7);

protected:
	bool isInputUnique( const Udm::Object& struct_47c0, const Udm::Object& member_47c9, const Udm::Object& triggerVar_47d2, const Udm::Object& unaryExprs_47db);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& structs_47ac, const Packets_t& members_47af, const Packets_t& triggerVars_47b2, const Packets_t& unaryExprss_47b5);
	bool patternMatcher( const Udm::Object& struct_47be, const Udm::Object& member_47c7, const Udm::Object& triggerVar_47d0, const Udm::Object& unaryExprs_47d9);
	void outputAppender( const SFC::Struct& struct_47ec, const SFC::LocalVar& member_47ee, const SFC::LocalVar& triggerVar_47f0, const SFC::UnaryExprs& unaryExprs_47f2);

private:
	Packets_t* _struct_47b8;
	Packets_t* _member_47b9;
	Packets_t* _triggerVar_47ba;
	Packets_t* _unaryExprs_47bb;
	Packets_t _struct_47bc;
	Packets_t _member_47c5;
	Packets_t _triggerVar_47ce;
	Packets_t _unaryExprs_47d7;
	class Match
	{
	public:
		SFC::Struct struct_47e7;
		SFC::LocalVar member_47e8;
		SFC::LocalVar triggerVar_47e9;
		SFC::UnaryExprs unaryExprs_47ea;
		SFC::BinaryExprs binaryExprs_47eb;
	};

	std::list< Match> _matches;
};

class Otherwise_47f4
{
public:
	bool operator()( const Packets_t& structs_47f5, const Packets_t& members_47f8, const Packets_t& triggerVars_47fb, const Packets_t& unaryExprss_47fe, Packets_t& structs_47f7, Packets_t& members_47fa, Packets_t& triggerVars_47fd, Packets_t& unaryExprss_4800);

protected:
	bool isInputUnique( const Udm::Object& struct_4809, const Udm::Object& member_4812, const Udm::Object& triggerVar_481b, const Udm::Object& unaryExprs_4824);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& structs_47f5, const Packets_t& members_47f8, const Packets_t& triggerVars_47fb, const Packets_t& unaryExprss_47fe);
	bool patternMatcher( const Udm::Object& struct_4807, const Udm::Object& member_4810, const Udm::Object& triggerVar_4819, const Udm::Object& unaryExprs_4822);
	void outputAppender( const SFC::Struct& struct_4831, const SFC::LocalVar& member_4833, const SFC::LocalVar& triggerVar_4835, const SFC::UnaryExprs& unaryExprs_4837);

private:
	Packets_t* _struct_4801;
	Packets_t* _member_4802;
	Packets_t* _triggerVar_4803;
	Packets_t* _unaryExprs_4804;
	Packets_t _struct_4805;
	Packets_t _member_480e;
	Packets_t _triggerVar_4817;
	Packets_t _unaryExprs_4820;
	class Match
	{
	public:
		SFC::Struct struct_482d;
		SFC::LocalVar member_482e;
		SFC::LocalVar triggerVar_482f;
		SFC::UnaryExprs unaryExprs_4830;
	};

	std::list< Match> _matches;
};

class TraverseBinary_4839
{
public:
	void operator()( const Packets_t& structs_483a, const Packets_t& members_483d, const Packets_t& triggerVars_4840, const Packets_t& unaryExprss_4843, Packets_t& structs_483c, Packets_t& members_483f, Packets_t& triggerVars_4842, Packets_t& unaryExprss_4845);

protected:
	bool isInputUnique( const Udm::Object& struct_484e, const Udm::Object& member_4857, const Udm::Object& triggerVar_4860, const Udm::Object& unaryExprs_4869);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& structs_483a, const Packets_t& members_483d, const Packets_t& triggerVars_4840, const Packets_t& unaryExprss_4843);
	bool patternMatcher( const Udm::Object& struct_484c, const Udm::Object& member_4855, const Udm::Object& triggerVar_485e, const Udm::Object& unaryExprs_4867);
	void effector();
	void outputAppender( const SFC::Struct& struct_487e, const SFC::LocalVar& member_4880, const SFC::LocalVar& triggerVar_4882, const SFC::UnaryExprs& unaryExprs_4884);

private:
	Packets_t* _struct_4846;
	Packets_t* _member_4847;
	Packets_t* _triggerVar_4848;
	Packets_t* _unaryExprs_4849;
	Packets_t _struct_484a;
	Packets_t _member_4853;
	Packets_t _triggerVar_485c;
	Packets_t _unaryExprs_4865;
	class Match
	{
	public:
		SFC::Struct struct_4878;
		SFC::LocalVar member_4879;
		SFC::LocalVar triggerVar_487a;
		SFC::UnaryExprs unaryExprs_487b;
		SFC::BinaryExprs binaryExprs_487c;
		SFC::UnaryExprs unaryExprs_487d;
	};

	std::list< Match> _matches;
};

class GetStructMembers_4895
{
public:
	void operator()( const Packets_t& structs_4896, const Packets_t& triggerVars_489a, const Packets_t& unaryExprss_489d, Packets_t& structs_4898, Packets_t& members_4899, Packets_t& triggerVars_489c, Packets_t& unaryExprss_489f);

protected:
	bool isInputUnique( const Udm::Object& struct_48a8, const Udm::Object& triggerVar_48b1, const Udm::Object& unaryExprs_48ba);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& structs_4896, const Packets_t& triggerVars_489a, const Packets_t& unaryExprss_489d);
	bool patternMatcher( const Udm::Object& struct_48a6, const Udm::Object& triggerVar_48af, const Udm::Object& unaryExprs_48b8);
	void effector();
	void outputAppender( const SFC::Struct& struct_48ca, const SFC::LocalVar& member_48cc, const SFC::LocalVar& triggerVar_48ce, const SFC::UnaryExprs& unaryExprs_48d0);
	void sortOutputs();

private:
	Packets_t* _struct_48a0;
	Packets_t* _member_48a1;
	Packets_t* _triggerVar_48a2;
	Packets_t* _unaryExprs_48a3;
	Packets_t _struct_48a4;
	Packets_t _triggerVar_48ad;
	Packets_t _unaryExprs_48b6;
	class Match
	{
	public:
		SFC::Struct struct_48c6;
		SFC::LocalVar triggerVar_48c7;
		SFC::UnaryExprs unaryExprs_48c8;
		SFC::LocalVar member_48c9;
	};

	std::list< Match> _matches;
};

class SetMatrixCondition_48df
{
public:
	void operator()( const Packets_t& dts_48e0, const Packets_t& triggervars_48e2, const Packets_t& conditions_48e4, Packets_t& dts_48e6, Packets_t& triggervars_48e7, Packets_t& conditions_48e8);

protected:
	void callMatrixCondition_4919( const Packets_t& dTs_48ed, const Packets_t& triggerVars_48ef, const Packets_t& conditions_48f1);

private:
	Packets_t* _dt_48e9;
	Packets_t* _triggervar_48ea;
	Packets_t* _condition_48eb;
};

class MatrixCondition_48ec
{
public:
	void operator()( const Packets_t& dTs_48ed, const Packets_t& triggerVars_48ef, const Packets_t& conditions_48f1);

protected:
	bool isInputUnique( const Udm::Object& dT_48f7, const Udm::Object& triggerVar_4900, const Udm::Object& condition_4909);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::UserCode& Condition, SFC::DT& DT, SFC::LocalVar& TriggerVar);
	void processInputPackets( const Packets_t& dTs_48ed, const Packets_t& triggerVars_48ef, const Packets_t& conditions_48f1);
	bool patternMatcher( const Udm::Object& dT_48f5, const Udm::Object& triggerVar_48fe, const Udm::Object& condition_4907);
	void effector();

private:
	Packets_t _dT_48f3;
	Packets_t _triggerVar_48fc;
	Packets_t _condition_4905;
	class Match
	{
	public:
		SFC::DT dT_4912;
		SFC::LocalVar triggerVar_4913;
		SFC::UserCode condition_4914;
	};

	std::list< Match> _matches;
};

class CreateConditional_491d
{
public:
	void operator()( const Packets_t& subsystems_491e, const Packets_t& charts_4921, const Packets_t& dTs_4924, const Packets_t& localVars_4927, const Packets_t& functions_492b, Packets_t& subsystems_4920, Packets_t& charts_4923, Packets_t& dTs_4926, Packets_t& triggerVars_4929, Packets_t& userCodes_492a, Packets_t& conditionalBlocks_492d);

protected:
	bool isInputUnique( const Udm::Object& subsystem_4938, const Udm::Object& chart_4941, const Udm::Object& dT_494a, const Udm::Object& localVar_4953, const Udm::Object& function_495c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_491e, const Packets_t& charts_4921, const Packets_t& dTs_4924, const Packets_t& localVars_4927, const Packets_t& functions_492b);
	bool patternMatcher( const Udm::Object& subsystem_4936, const Udm::Object& chart_493f, const Udm::Object& dT_4948, const Udm::Object& localVar_4951, const Udm::Object& function_495a);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_496e, const CyberComposition::Simulink::Subsystem& chart_4970, const SFC::DT& dT_4972, const SFC::LocalVar& triggerVar_4974, const SFC::UserCode& userCode_4976, const SFC::ConditionalBlock& conditionalBlock_4978);

private:
	Packets_t* _subsystem_492e;
	Packets_t* _chart_492f;
	Packets_t* _dT_4930;
	Packets_t* _triggerVar_4931;
	Packets_t* _userCode_4932;
	Packets_t* _conditionalBlock_4933;
	Packets_t _subsystem_4934;
	Packets_t _chart_493d;
	Packets_t _dT_4946;
	Packets_t _localVar_494f;
	Packets_t _function_4958;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_4966;
		CyberComposition::Simulink::Subsystem chart_4967;
		SFC::DT dT_4968;
		SFC::LocalVar localVar_4969;
		SFC::Function function_496a;
		SFC::LocalVar triggerVar_496b;
	};

	std::list< Match> _matches;
};

class AddStateVar_4988
{
public:
	void operator()( const Packets_t& subsystems_4989, const Packets_t& triggerPorts_498c, const Packets_t& argDeclBases_498f, const Packets_t& dTs_4993, const Packets_t& functions_4997, Packets_t& subsystems_498b, Packets_t& charts_498e, Packets_t& argDeclBases_4991, Packets_t& localVars_4992, Packets_t& dTs_4995, Packets_t& args_4996, Packets_t& functions_4999);

protected:
	bool isInputUnique( const Udm::Object& subsystem_49a5, const Udm::Object& triggerPort_49ae, const Udm::Object& argDeclBase_49b7, const Udm::Object& dT_49c0, const Udm::Object& function_49c9);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_4989, const Packets_t& triggerPorts_498c, const Packets_t& argDeclBases_498f, const Packets_t& dTs_4993, const Packets_t& functions_4997);
	bool patternMatcher( const Udm::Object& subsystem_49a3, const Udm::Object& triggerPort_49ac, const Udm::Object& argDeclBase_49b5, const Udm::Object& dT_49be, const Udm::Object& function_49c7);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_49e8, const CyberComposition::Simulink::Subsystem& chart_49ea, const SFC::ArgDeclBase& argDeclBase_49ec, const SFC::LocalVar& localVar_49ee, const SFC::DT& dT_49f0, const SFC::Arg& arg_49f2, const SFC::Function& function_49f4);

private:
	Packets_t* _subsystem_499a;
	Packets_t* _chart_499b;
	Packets_t* _argDeclBase_499c;
	Packets_t* _localVar_499d;
	Packets_t* _dT_499e;
	Packets_t* _arg_499f;
	Packets_t* _function_49a0;
	Packets_t _subsystem_49a1;
	Packets_t _triggerPort_49aa;
	Packets_t _argDeclBase_49b3;
	Packets_t _dT_49bc;
	Packets_t _function_49c5;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_49de;
		CyberComposition::Simulink::TriggerPort triggerPort_49df;
		SFC::ArgDeclBase argDeclBase_49e0;
		SFC::DT dT_49e1;
		SFC::Function function_49e2;
		CyberComposition::Simulink::Subsystem chart_49e3;
		SFC::Arg arg_49e4;
		SFC::Struct struct_49e5;
		SFC::Class class_49e6;
	};

	std::list< Match> _matches;
};

class SaveState_49f6
{
public:
	void operator()( const Packets_t& systems_49f7, const Packets_t& childsystems_49f9, const Packets_t& argdecls_49fb, const Packets_t& dts_49fd, const Packets_t& oldvals_49ff, const Packets_t& contexts_4a01, const Packets_t& functions_4a03, Packets_t& systems_4a05, Packets_t& childsystems_4a06, Packets_t& dts_4a07, Packets_t& oldvals_4a08, Packets_t& functions_4a09);

protected:
	void callSaveMatrixState_4ad6( const Packets_t& argdecls_4a70, const Packets_t& dts_4a72, const Packets_t& oldvals_4a74, const Packets_t& contexts_4a76, const Packets_t& functions_4a78);
	void callSaveStructState_4adc( const Packets_t& argdecls_4a10, const Packets_t& dts_4a12, const Packets_t& oldvals_4a14, const Packets_t& contexts_4a16, const Packets_t& functions_4a18);

private:
	Packets_t* _system_4a0a;
	Packets_t* _childsystem_4a0b;
	Packets_t* _dt_4a0c;
	Packets_t* _oldval_4a0d;
	Packets_t* _function_4a0e;
};

class SaveStructState_4a0f
{
public:
	void operator()( const Packets_t& argdecls_4a10, const Packets_t& dts_4a12, const Packets_t& oldvals_4a14, const Packets_t& contexts_4a16, const Packets_t& functions_4a18);

protected:
	void callSaveStructState_4a69( const Packets_t& argDeclBases_4a1b, const Packets_t& structs_4a1d, const Packets_t& localVars_4a1f, const Packets_t& contexts_4a21, const Packets_t& functions_4a23);
};

class SaveStructState_4a1a
{
public:
	void operator()( const Packets_t& argDeclBases_4a1b, const Packets_t& structs_4a1d, const Packets_t& localVars_4a1f, const Packets_t& contexts_4a21, const Packets_t& functions_4a23);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_4a29, const Udm::Object& struct_4a32, const Udm::Object& localVar_4a3b, const Udm::Object& context_4a44, const Udm::Object& function_4a4d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_4a1b, const Packets_t& structs_4a1d, const Packets_t& localVars_4a1f, const Packets_t& contexts_4a21, const Packets_t& functions_4a23);
	bool patternMatcher( const Udm::Object& argDeclBase_4a27, const Udm::Object& struct_4a30, const Udm::Object& localVar_4a39, const Udm::Object& context_4a42, const Udm::Object& function_4a4b);
	void effector();

private:
	Packets_t _argDeclBase_4a25;
	Packets_t _struct_4a2e;
	Packets_t _localVar_4a37;
	Packets_t _context_4a40;
	Packets_t _function_4a49;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_4a59;
		SFC::Struct struct_4a5a;
		SFC::LocalVar localVar_4a5b;
		SFC::Arg context_4a5c;
		SFC::Function function_4a5d;
		SFC::LocalVar member_4a5e;
	};

	std::list< Match> _matches;
};

class SaveMatrixState_4a6f
{
public:
	void operator()( const Packets_t& argdecls_4a70, const Packets_t& dts_4a72, const Packets_t& oldvals_4a74, const Packets_t& contexts_4a76, const Packets_t& functions_4a78, Packets_t& argdecls_4a7a, Packets_t& dts_4a7b, Packets_t& oldvals_4a7c, Packets_t& contexts_4a7d, Packets_t& functions_4a7e);

protected:
	void callSaveMatrixState_4ad0( const Packets_t& argDeclBases_4a85, const Packets_t& dTs_4a87, const Packets_t& localVars_4a89, const Packets_t& args_4a8b, const Packets_t& functions_4a8d);

private:
	Packets_t* _argdecl_4a7f;
	Packets_t* _dt_4a80;
	Packets_t* _oldval_4a81;
	Packets_t* _context_4a82;
	Packets_t* _function_4a83;
};

class SaveMatrixState_4a84
{
public:
	void operator()( const Packets_t& argDeclBases_4a85, const Packets_t& dTs_4a87, const Packets_t& localVars_4a89, const Packets_t& args_4a8b, const Packets_t& functions_4a8d);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_4a93, const Udm::Object& dT_4a9c, const Udm::Object& localVar_4aa5, const Udm::Object& arg_4aae, const Udm::Object& function_4ab7);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg, SFC::ArgDeclBase& ArgDeclBase, SFC::DT& DT, SFC::Function& Function, SFC::LocalVar& LocalVar);
	void processInputPackets( const Packets_t& argDeclBases_4a85, const Packets_t& dTs_4a87, const Packets_t& localVars_4a89, const Packets_t& args_4a8b, const Packets_t& functions_4a8d);
	bool patternMatcher( const Udm::Object& argDeclBase_4a91, const Udm::Object& dT_4a9a, const Udm::Object& localVar_4aa3, const Udm::Object& arg_4aac, const Udm::Object& function_4ab5);
	void effector();

private:
	Packets_t _argDeclBase_4a8f;
	Packets_t _dT_4a98;
	Packets_t _localVar_4aa1;
	Packets_t _arg_4aaa;
	Packets_t _function_4ab3;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_4ac0;
		SFC::DT dT_4ac1;
		SFC::LocalVar localVar_4ac2;
		SFC::Arg arg_4ac3;
		SFC::Function function_4ac4;
	};

	std::list< Match> _matches;
};

class AddTriggerVars_4ae2
{
public:
	void operator()( const Packets_t& systems_4ae3, const Packets_t& childsystems_4ae5, const Packets_t& argdecls_4ae7, const Packets_t& dts_4ae9, const Packets_t& oldvals_4aeb, const Packets_t& contexts_4aed, const Packets_t& functions_4aef, Packets_t& systems_4af1, Packets_t& childsystems_4af2, Packets_t& argdecls_4af3, Packets_t& dts_4af4, Packets_t& oldvals_4af5, Packets_t& contexts_4af6, Packets_t& functions_4af7);

protected:
	void callAddMatrixTriggerVars_51fa( const Packets_t& childSubsystems_4ebf, const Packets_t& argdecls_4ec1, const Packets_t& dts_4ec3, const Packets_t& oldvals_4ec5, const Packets_t& contexts_4ec7, const Packets_t& functions_4ec9);
	void callAddStructTriggerVars_5201( const Packets_t& childSubsystems_4b00, const Packets_t& argdecls_4b02, const Packets_t& dts_4b04, const Packets_t& oldvals_4b06, const Packets_t& contexts_4b08, const Packets_t& functions_4b0a);

private:
	Packets_t* _system_4af8;
	Packets_t* _childsystem_4af9;
	Packets_t* _argdecl_4afa;
	Packets_t* _dt_4afb;
	Packets_t* _oldval_4afc;
	Packets_t* _context_4afd;
	Packets_t* _function_4afe;
};

class AddStructTriggerVars_4aff
{
public:
	void operator()( const Packets_t& childSubsystems_4b00, const Packets_t& argdecls_4b02, const Packets_t& dts_4b04, const Packets_t& oldvals_4b06, const Packets_t& contexts_4b08, const Packets_t& functions_4b0a);

protected:
	void callAddTriggerVar_4e8a( const Packets_t& charts_4d8f, const Packets_t& argDeclBases_4d92, const Packets_t& structs_4d95, const Packets_t& localVars_4d98, const Packets_t& contexts_4d9b, const Packets_t& functions_4d9f);
	void callStartAssignment_4e91( const Packets_t& charts_4dfa, const Packets_t& argDeclBases_4dfc, const Packets_t& structs_4e00, const Packets_t& oldvals_4e02, const Packets_t& contexts_4e05, const Packets_t& triggerVars_4e08, const Packets_t& functions_4e0a);
	void callEdgeTest_4e99( const Packets_t& events_4c05, const Packets_t& argdecls_4c07, const Packets_t& members_4c09, const Packets_t& oldvals_4c0b, const Packets_t& contexts_4c0d, const Packets_t& initexprs_4c0f);
	void callEitherEdge_4ea0( const Packets_t& argDeclBases_4b0d, const Packets_t& members_4b10, const Packets_t& localVars_4b13, const Packets_t& contexts_4b16, const Packets_t& unaryExprss_4b19);
	void callFallingEdge_4ea6( const Packets_t& argDeclBases_4b69, const Packets_t& members_4b6b, const Packets_t& localVars_4b6d, const Packets_t& contexts_4b6f, const Packets_t& unaryExprss_4b71);
	void callRisingEdge_4eac( const Packets_t& argDeclBases_4bb7, const Packets_t& members_4bb9, const Packets_t& localVars_4bbb, const Packets_t& contexts_4bbd, const Packets_t& unaryExprss_4bbf);
	void callFallingEdge_4eb2( const Packets_t& argDeclBases_4b69, const Packets_t& members_4b6b, const Packets_t& localVars_4b6d, const Packets_t& contexts_4b6f, const Packets_t& unaryExprss_4b71);
	void callRisingEdge_4eb8( const Packets_t& argDeclBases_4bb7, const Packets_t& members_4bb9, const Packets_t& localVars_4bbb, const Packets_t& contexts_4bbd, const Packets_t& unaryExprss_4bbf);
};

class EitherEdge_4b0c
{
public:
	void operator()( const Packets_t& argDeclBases_4b0d, const Packets_t& members_4b10, const Packets_t& localVars_4b13, const Packets_t& contexts_4b16, const Packets_t& unaryExprss_4b19, Packets_t& argDeclBases_4b0f, Packets_t& members_4b12, Packets_t& localVars_4b15, Packets_t& contexts_4b18, Packets_t& leftUnaryExprss_4b1b, Packets_t& rightUnaryExprss_4b1c);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_4b27, const Udm::Object& member_4b30, const Udm::Object& localVar_4b39, const Udm::Object& context_4b42, const Udm::Object& unaryExprs_4b4b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_4b0d, const Packets_t& members_4b10, const Packets_t& localVars_4b13, const Packets_t& contexts_4b16, const Packets_t& unaryExprss_4b19);
	bool patternMatcher( const Udm::Object& argDeclBase_4b25, const Udm::Object& member_4b2e, const Udm::Object& localVar_4b37, const Udm::Object& context_4b40, const Udm::Object& unaryExprs_4b49);
	void effector();
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_4b5c, const SFC::LocalVar& member_4b5e, const SFC::LocalVar& localVar_4b60, const SFC::Arg& context_4b62, const SFC::UnaryExprs& leftUnaryExprs_4b64, const SFC::UnaryExprs& rightUnaryExprs_4b66);

private:
	Packets_t* _argDeclBase_4b1d;
	Packets_t* _member_4b1e;
	Packets_t* _localVar_4b1f;
	Packets_t* _context_4b20;
	Packets_t* _leftUnaryExprs_4b21;
	Packets_t* _rightUnaryExprs_4b22;
	Packets_t _argDeclBase_4b23;
	Packets_t _member_4b2c;
	Packets_t _localVar_4b35;
	Packets_t _context_4b3e;
	Packets_t _unaryExprs_4b47;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_4b54;
		SFC::LocalVar member_4b55;
		SFC::LocalVar localVar_4b56;
		SFC::Arg context_4b57;
		SFC::UnaryExprs unaryExprs_4b58;
	};

	std::list< Match> _matches;
};

class FallingEdge_4b68
{
public:
	void operator()( const Packets_t& argDeclBases_4b69, const Packets_t& members_4b6b, const Packets_t& localVars_4b6d, const Packets_t& contexts_4b6f, const Packets_t& unaryExprss_4b71);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_4b77, const Udm::Object& member_4b80, const Udm::Object& localVar_4b89, const Udm::Object& context_4b92, const Udm::Object& unaryExprs_4b9b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_4b69, const Packets_t& members_4b6b, const Packets_t& localVars_4b6d, const Packets_t& contexts_4b6f, const Packets_t& unaryExprss_4b71);
	bool patternMatcher( const Udm::Object& argDeclBase_4b75, const Udm::Object& member_4b7e, const Udm::Object& localVar_4b87, const Udm::Object& context_4b90, const Udm::Object& unaryExprs_4b99);
	void effector();

private:
	Packets_t _argDeclBase_4b73;
	Packets_t _member_4b7c;
	Packets_t _localVar_4b85;
	Packets_t _context_4b8e;
	Packets_t _unaryExprs_4b97;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_4ba4;
		SFC::LocalVar member_4ba5;
		SFC::LocalVar localVar_4ba6;
		SFC::Arg context_4ba7;
		SFC::UnaryExprs unaryExprs_4ba8;
	};

	std::list< Match> _matches;
};

class RisingEdge_4bb6
{
public:
	void operator()( const Packets_t& argDeclBases_4bb7, const Packets_t& members_4bb9, const Packets_t& localVars_4bbb, const Packets_t& contexts_4bbd, const Packets_t& unaryExprss_4bbf);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_4bc5, const Udm::Object& member_4bce, const Udm::Object& localVar_4bd7, const Udm::Object& context_4be0, const Udm::Object& unaryExprs_4be9);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_4bb7, const Packets_t& members_4bb9, const Packets_t& localVars_4bbb, const Packets_t& contexts_4bbd, const Packets_t& unaryExprss_4bbf);
	bool patternMatcher( const Udm::Object& argDeclBase_4bc3, const Udm::Object& member_4bcc, const Udm::Object& localVar_4bd5, const Udm::Object& context_4bde, const Udm::Object& unaryExprs_4be7);
	void effector();

private:
	Packets_t _argDeclBase_4bc1;
	Packets_t _member_4bca;
	Packets_t _localVar_4bd3;
	Packets_t _context_4bdc;
	Packets_t _unaryExprs_4be5;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_4bf2;
		SFC::LocalVar member_4bf3;
		SFC::LocalVar localVar_4bf4;
		SFC::Arg context_4bf5;
		SFC::UnaryExprs unaryExprs_4bf6;
	};

	std::list< Match> _matches;
};

class EdgeTest_4c04
{
public:
	void operator()( const Packets_t& events_4c05, const Packets_t& argdecls_4c07, const Packets_t& members_4c09, const Packets_t& oldvals_4c0b, const Packets_t& contexts_4c0d, const Packets_t& initexprs_4c0f, Packets_t& argdecls_4c11, Packets_t& members_4c12, Packets_t& oldvals_4c13, Packets_t& contexts_4c14, Packets_t& initexprs_4c15, Packets_t& argdecls_4c16, Packets_t& members_4c17, Packets_t& oldvals_4c18, Packets_t& contexts_4c19, Packets_t& initexprs_4c1a, Packets_t& argdecls_4c1b, Packets_t& members_4c1c, Packets_t& oldvals_4c1d, Packets_t& contexts_4c1e, Packets_t& initexprs_4c1f);

protected:
	void executeOne( const Packets_t& events_4c05, const Packets_t& argdecls_4c07, const Packets_t& members_4c09, const Packets_t& oldvals_4c0b, const Packets_t& contexts_4c0d, const Packets_t& initexprs_4c0f);
	bool isInputUnique( const Udm::Object& event_4c31, const Udm::Object& argdecl_4c38, const Udm::Object& member_4c3f, const Udm::Object& oldval_4c46, const Udm::Object& context_4c4d, const Udm::Object& initexpr_4c54);

private:
	Packets_t* _argdecl_4c20;
	Packets_t* _member_4c21;
	Packets_t* _oldval_4c22;
	Packets_t* _context_4c23;
	Packets_t* _initexpr_4c24;
	Packets_t* _argdecl_4c25;
	Packets_t* _member_4c26;
	Packets_t* _oldval_4c27;
	Packets_t* _context_4c28;
	Packets_t* _initexpr_4c29;
	Packets_t* _argdecl_4c2a;
	Packets_t* _member_4c2b;
	Packets_t* _oldval_4c2c;
	Packets_t* _context_4c2d;
	Packets_t* _initexpr_4c2e;
	Packets_t _event_4c2f;
	Packets_t _argdecl_4c36;
	Packets_t _member_4c3d;
	Packets_t _oldval_4c44;
	Packets_t _context_4c4b;
	Packets_t _initexpr_4c52;
};

class RisingEdge_4c59
{
public:
	bool operator()( const Packets_t& events_4c5a, const Packets_t& argDeclBases_4c5c, const Packets_t& members_4c5f, const Packets_t& localVars_4c62, const Packets_t& contexts_4c65, const Packets_t& unaryExprss_4c68, Packets_t& argDeclBases_4c5e, Packets_t& members_4c61, Packets_t& localVars_4c64, Packets_t& contexts_4c67, Packets_t& unaryExprss_4c6a);

protected:
	bool isInputUnique( const Udm::Object& event_4c74, const Udm::Object& argDeclBase_4c7d, const Udm::Object& member_4c86, const Udm::Object& localVar_4c8f, const Udm::Object& context_4c98, const Udm::Object& unaryExprs_4ca1);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::Arg& Context, CyberComposition::Simulink::Event& Event, SFC::LocalVar& LocalVar, SFC::LocalVar& Member, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& events_4c5a, const Packets_t& argDeclBases_4c5c, const Packets_t& members_4c5f, const Packets_t& localVars_4c62, const Packets_t& contexts_4c65, const Packets_t& unaryExprss_4c68);
	bool patternMatcher( const Udm::Object& event_4c72, const Udm::Object& argDeclBase_4c7b, const Udm::Object& member_4c84, const Udm::Object& localVar_4c8d, const Udm::Object& context_4c96, const Udm::Object& unaryExprs_4c9f);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_4cb6, const SFC::LocalVar& member_4cb8, const SFC::LocalVar& localVar_4cba, const SFC::Arg& context_4cbc, const SFC::UnaryExprs& unaryExprs_4cbe);

private:
	Packets_t* _argDeclBase_4c6b;
	Packets_t* _member_4c6c;
	Packets_t* _localVar_4c6d;
	Packets_t* _context_4c6e;
	Packets_t* _unaryExprs_4c6f;
	Packets_t _event_4c70;
	Packets_t _argDeclBase_4c79;
	Packets_t _member_4c82;
	Packets_t _localVar_4c8b;
	Packets_t _context_4c94;
	Packets_t _unaryExprs_4c9d;
	class Match
	{
	public:
		CyberComposition::Simulink::Event event_4caa;
		SFC::ArgDeclBase argDeclBase_4cab;
		SFC::LocalVar member_4cac;
		SFC::LocalVar localVar_4cad;
		SFC::Arg context_4cae;
		SFC::UnaryExprs unaryExprs_4caf;
	};

	std::list< Match> _matches;
};

class FallingEdge_4cc0
{
public:
	bool operator()( const Packets_t& events_4cc1, const Packets_t& argDeclBases_4cc3, const Packets_t& members_4cc6, const Packets_t& localVars_4cc9, const Packets_t& contexts_4ccc, const Packets_t& unaryExprss_4ccf, Packets_t& argDeclBases_4cc5, Packets_t& members_4cc8, Packets_t& localVars_4ccb, Packets_t& contexts_4cce, Packets_t& unaryExprss_4cd1);

protected:
	bool isInputUnique( const Udm::Object& event_4cdb, const Udm::Object& argDeclBase_4ce4, const Udm::Object& member_4ced, const Udm::Object& localVar_4cf6, const Udm::Object& context_4cff, const Udm::Object& unaryExprs_4d08);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::Arg& Context, CyberComposition::Simulink::Event& Event, SFC::LocalVar& LocalVar, SFC::LocalVar& Member, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& events_4cc1, const Packets_t& argDeclBases_4cc3, const Packets_t& members_4cc6, const Packets_t& localVars_4cc9, const Packets_t& contexts_4ccc, const Packets_t& unaryExprss_4ccf);
	bool patternMatcher( const Udm::Object& event_4cd9, const Udm::Object& argDeclBase_4ce2, const Udm::Object& member_4ceb, const Udm::Object& localVar_4cf4, const Udm::Object& context_4cfd, const Udm::Object& unaryExprs_4d06);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_4d1d, const SFC::LocalVar& member_4d1f, const SFC::LocalVar& localVar_4d21, const SFC::Arg& context_4d23, const SFC::UnaryExprs& unaryExprs_4d25);

private:
	Packets_t* _argDeclBase_4cd2;
	Packets_t* _member_4cd3;
	Packets_t* _localVar_4cd4;
	Packets_t* _context_4cd5;
	Packets_t* _unaryExprs_4cd6;
	Packets_t _event_4cd7;
	Packets_t _argDeclBase_4ce0;
	Packets_t _member_4ce9;
	Packets_t _localVar_4cf2;
	Packets_t _context_4cfb;
	Packets_t _unaryExprs_4d04;
	class Match
	{
	public:
		CyberComposition::Simulink::Event event_4d11;
		SFC::ArgDeclBase argDeclBase_4d12;
		SFC::LocalVar member_4d13;
		SFC::LocalVar localVar_4d14;
		SFC::Arg context_4d15;
		SFC::UnaryExprs unaryExprs_4d16;
	};

	std::list< Match> _matches;
};

class EitherEdge_4d27
{
public:
	bool operator()( const Packets_t& events_4d28, const Packets_t& argDeclBases_4d2a, const Packets_t& members_4d2d, const Packets_t& localVars_4d30, const Packets_t& contexts_4d33, const Packets_t& unaryExprss_4d36, Packets_t& argDeclBases_4d2c, Packets_t& members_4d2f, Packets_t& localVars_4d32, Packets_t& contexts_4d35, Packets_t& unaryExprss_4d38);

protected:
	bool isInputUnique( const Udm::Object& event_4d42, const Udm::Object& argDeclBase_4d4b, const Udm::Object& member_4d54, const Udm::Object& localVar_4d5d, const Udm::Object& context_4d66, const Udm::Object& unaryExprs_4d6f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::Arg& Context, CyberComposition::Simulink::Event& Event, SFC::LocalVar& LocalVar, SFC::LocalVar& Member, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& events_4d28, const Packets_t& argDeclBases_4d2a, const Packets_t& members_4d2d, const Packets_t& localVars_4d30, const Packets_t& contexts_4d33, const Packets_t& unaryExprss_4d36);
	bool patternMatcher( const Udm::Object& event_4d40, const Udm::Object& argDeclBase_4d49, const Udm::Object& member_4d52, const Udm::Object& localVar_4d5b, const Udm::Object& context_4d64, const Udm::Object& unaryExprs_4d6d);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_4d84, const SFC::LocalVar& member_4d86, const SFC::LocalVar& localVar_4d88, const SFC::Arg& context_4d8a, const SFC::UnaryExprs& unaryExprs_4d8c);

private:
	Packets_t* _argDeclBase_4d39;
	Packets_t* _member_4d3a;
	Packets_t* _localVar_4d3b;
	Packets_t* _context_4d3c;
	Packets_t* _unaryExprs_4d3d;
	Packets_t _event_4d3e;
	Packets_t _argDeclBase_4d47;
	Packets_t _member_4d50;
	Packets_t _localVar_4d59;
	Packets_t _context_4d62;
	Packets_t _unaryExprs_4d6b;
	class Match
	{
	public:
		CyberComposition::Simulink::Event event_4d78;
		SFC::ArgDeclBase argDeclBase_4d79;
		SFC::LocalVar member_4d7a;
		SFC::LocalVar localVar_4d7b;
		SFC::Arg context_4d7c;
		SFC::UnaryExprs unaryExprs_4d7d;
	};

	std::list< Match> _matches;
};

class AddTriggerVar_4d8e
{
public:
	void operator()( const Packets_t& charts_4d8f, const Packets_t& argDeclBases_4d92, const Packets_t& structs_4d95, const Packets_t& localVars_4d98, const Packets_t& contexts_4d9b, const Packets_t& functions_4d9f, Packets_t& charts_4d91, Packets_t& argDeclBases_4d94, Packets_t& structs_4d97, Packets_t& localVars_4d9a, Packets_t& contexts_4d9d, Packets_t& triggerVars_4d9e, Packets_t& functions_4da1);

protected:
	bool isInputUnique( const Udm::Object& chart_4dad, const Udm::Object& argDeclBase_4db6, const Udm::Object& struct_4dbf, const Udm::Object& localVar_4dc8, const Udm::Object& context_4dd1, const Udm::Object& function_4dda);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& charts_4d8f, const Packets_t& argDeclBases_4d92, const Packets_t& structs_4d95, const Packets_t& localVars_4d98, const Packets_t& contexts_4d9b, const Packets_t& functions_4d9f);
	bool patternMatcher( const Udm::Object& chart_4dab, const Udm::Object& argDeclBase_4db4, const Udm::Object& struct_4dbd, const Udm::Object& localVar_4dc6, const Udm::Object& context_4dcf, const Udm::Object& function_4dd8);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& chart_4dea, const SFC::ArgDeclBase& argDeclBase_4dec, const SFC::Struct& struct_4dee, const SFC::LocalVar& localVar_4df0, const SFC::Arg& context_4df2, const SFC::LocalVar& triggerVar_4df4, const SFC::Function& function_4df6);

private:
	Packets_t* _chart_4da2;
	Packets_t* _argDeclBase_4da3;
	Packets_t* _struct_4da4;
	Packets_t* _localVar_4da5;
	Packets_t* _context_4da6;
	Packets_t* _triggerVar_4da7;
	Packets_t* _function_4da8;
	Packets_t _chart_4da9;
	Packets_t _argDeclBase_4db2;
	Packets_t _struct_4dbb;
	Packets_t _localVar_4dc4;
	Packets_t _context_4dcd;
	Packets_t _function_4dd6;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem chart_4de3;
		SFC::ArgDeclBase argDeclBase_4de4;
		SFC::Struct struct_4de5;
		SFC::LocalVar localVar_4de6;
		SFC::Arg context_4de7;
		SFC::Function function_4de8;
	};

	std::list< Match> _matches;
};

class StartAssignment_4df8
{
public:
	void operator()( const Packets_t& charts_4dfa, const Packets_t& argDeclBases_4dfc, const Packets_t& structs_4e00, const Packets_t& oldvals_4e02, const Packets_t& contexts_4e05, const Packets_t& triggerVars_4e08, const Packets_t& functions_4e0a, Packets_t& events_4df9, Packets_t& argDeclBases_4dfe, Packets_t& localVars_4dff, Packets_t& oldvals_4e04, Packets_t& contexts_4e07, Packets_t& unaryExprss_4e0c);

protected:
	bool isInputUnique( const Udm::Object& chart_4e17, const Udm::Object& argDeclBase_4e20, const Udm::Object& struct_4e29, const Udm::Object& oldval_4e32, const Udm::Object& context_4e3b, const Udm::Object& triggerVar_4e44, const Udm::Object& function_4e4d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, CyberComposition::Simulink::Subsystem& Chart, SFC::Arg& Context, CyberComposition::Simulink::Event& Event, SFC::Function& Function, SFC::LocalVar& LocalVar, CyberComposition::Simulink::Primitive& Primitive, CyberComposition::Simulink::State& State, SFC::Struct& Struct, SFC::LocalVar& TriggerVar, SFC::LocalVar& oldval);
	void processInputPackets( const Packets_t& charts_4dfa, const Packets_t& argDeclBases_4dfc, const Packets_t& structs_4e00, const Packets_t& oldvals_4e02, const Packets_t& contexts_4e05, const Packets_t& triggerVars_4e08, const Packets_t& functions_4e0a);
	bool patternMatcher( const Udm::Object& chart_4e15, const Udm::Object& argDeclBase_4e1e, const Udm::Object& struct_4e27, const Udm::Object& oldval_4e30, const Udm::Object& context_4e39, const Udm::Object& triggerVar_4e42, const Udm::Object& function_4e4b);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Event& event_4e7e, const SFC::ArgDeclBase& argDeclBase_4e80, const SFC::LocalVar& localVar_4e82, const SFC::LocalVar& oldval_4e84, const SFC::Arg& context_4e86, const SFC::UnaryExprs& unaryExprs_4e88);

private:
	Packets_t* _event_4e0d;
	Packets_t* _argDeclBase_4e0e;
	Packets_t* _localVar_4e0f;
	Packets_t* _oldval_4e10;
	Packets_t* _context_4e11;
	Packets_t* _unaryExprs_4e12;
	Packets_t _chart_4e13;
	Packets_t _argDeclBase_4e1c;
	Packets_t _struct_4e25;
	Packets_t _oldval_4e2e;
	Packets_t _context_4e37;
	Packets_t _triggerVar_4e40;
	Packets_t _function_4e49;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem chart_4e62;
		SFC::ArgDeclBase argDeclBase_4e63;
		SFC::Struct struct_4e64;
		SFC::LocalVar oldval_4e65;
		SFC::Arg context_4e66;
		SFC::LocalVar triggerVar_4e67;
		SFC::Function function_4e68;
		CyberComposition::Simulink::State state_4e69;
		CyberComposition::Simulink::Event event_4e6a;
		CyberComposition::Simulink::Primitive primitive_4e6b;
		SFC::LocalVar localVar_4e6c;
	};

	std::list< Match> _matches;
};

class AddMatrixTriggerVars_4ebe
{
public:
	void operator()( const Packets_t& childSubsystems_4ebf, const Packets_t& argdecls_4ec1, const Packets_t& dts_4ec3, const Packets_t& oldvals_4ec5, const Packets_t& contexts_4ec7, const Packets_t& functions_4ec9, Packets_t& childSubsystems_4ecb, Packets_t& argdecls_4ecc, Packets_t& dts_4ecd, Packets_t& oldvals_4ece, Packets_t& contexts_4ecf, Packets_t& functions_4ed0);

protected:
	void callAddTriggerVar_51cd( const Packets_t& charts_4f16, const Packets_t& argDeclBases_4f19, const Packets_t& dTs_4f1c, const Packets_t& localVars_4f1e, const Packets_t& contexts_4f21, const Packets_t& functions_4f25);
	void callStartAssignment_51d4( const Packets_t& charts_5155, const Packets_t& argDeclBases_5157, const Packets_t& oldvals_515a, const Packets_t& contexts_515d, const Packets_t& triggerVars_5160, const Packets_t& functions_5162);
	void callEdgeTest_51db( const Packets_t& events_4f82, const Packets_t& argdecls_4f84, const Packets_t& oldvals_4f86, const Packets_t& contexts_4f88, const Packets_t& initexprs_4f8a);
	void callFallingEdge_51e1( const Packets_t& argDeclBases_50ca, const Packets_t& localVars_50cc, const Packets_t& contexts_50ce, const Packets_t& unaryExprss_50d0);
	void callEitherEdge_51e6( const Packets_t& argDeclBases_5108, const Packets_t& localVars_510b, const Packets_t& contexts_510e, const Packets_t& unaryExprss_5111);
	void callRisingEdge_51eb( const Packets_t& argDeclBases_4ed8, const Packets_t& localVars_4eda, const Packets_t& contexts_4edc, const Packets_t& unaryExprss_4ede);
	void callRisingEdge_51f0( const Packets_t& argDeclBases_4ed8, const Packets_t& localVars_4eda, const Packets_t& contexts_4edc, const Packets_t& unaryExprss_4ede);
	void callFallingEdge_51f5( const Packets_t& argDeclBases_50ca, const Packets_t& localVars_50cc, const Packets_t& contexts_50ce, const Packets_t& unaryExprss_50d0);

private:
	Packets_t* _childSubsystem_4ed1;
	Packets_t* _argdecl_4ed2;
	Packets_t* _dt_4ed3;
	Packets_t* _oldval_4ed4;
	Packets_t* _context_4ed5;
	Packets_t* _function_4ed6;
};

class RisingEdge_4ed7
{
public:
	void operator()( const Packets_t& argDeclBases_4ed8, const Packets_t& localVars_4eda, const Packets_t& contexts_4edc, const Packets_t& unaryExprss_4ede);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_4ee4, const Udm::Object& localVar_4eed, const Udm::Object& context_4ef6, const Udm::Object& unaryExprs_4eff);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_4ed8, const Packets_t& localVars_4eda, const Packets_t& contexts_4edc, const Packets_t& unaryExprss_4ede);
	bool patternMatcher( const Udm::Object& argDeclBase_4ee2, const Udm::Object& localVar_4eeb, const Udm::Object& context_4ef4, const Udm::Object& unaryExprs_4efd);
	void effector();

private:
	Packets_t _argDeclBase_4ee0;
	Packets_t _localVar_4ee9;
	Packets_t _context_4ef2;
	Packets_t _unaryExprs_4efb;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_4f08;
		SFC::LocalVar localVar_4f09;
		SFC::Arg context_4f0a;
		SFC::UnaryExprs unaryExprs_4f0b;
	};

	std::list< Match> _matches;
};

class AddTriggerVar_4f15
{
public:
	void operator()( const Packets_t& charts_4f16, const Packets_t& argDeclBases_4f19, const Packets_t& dTs_4f1c, const Packets_t& localVars_4f1e, const Packets_t& contexts_4f21, const Packets_t& functions_4f25, Packets_t& charts_4f18, Packets_t& argDeclBases_4f1b, Packets_t& localVars_4f20, Packets_t& contexts_4f23, Packets_t& triggerVars_4f24, Packets_t& functions_4f27);

protected:
	bool isInputUnique( const Udm::Object& chart_4f32, const Udm::Object& argDeclBase_4f3b, const Udm::Object& dT_4f44, const Udm::Object& localVar_4f4d, const Udm::Object& context_4f56, const Udm::Object& function_4f5f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, CyberComposition::Simulink::Subsystem& Chart, SFC::Arg& Context, SFC::DT& DT, SFC::Function& Function, SFC::LocalVar& LocalVar);
	void processInputPackets( const Packets_t& charts_4f16, const Packets_t& argDeclBases_4f19, const Packets_t& dTs_4f1c, const Packets_t& localVars_4f1e, const Packets_t& contexts_4f21, const Packets_t& functions_4f25);
	bool patternMatcher( const Udm::Object& chart_4f30, const Udm::Object& argDeclBase_4f39, const Udm::Object& dT_4f42, const Udm::Object& localVar_4f4b, const Udm::Object& context_4f54, const Udm::Object& function_4f5d);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& chart_4f75, const SFC::ArgDeclBase& argDeclBase_4f77, const SFC::LocalVar& localVar_4f79, const SFC::Arg& context_4f7b, const SFC::LocalVar& triggerVar_4f7d, const SFC::Function& function_4f7f);

private:
	Packets_t* _chart_4f28;
	Packets_t* _argDeclBase_4f29;
	Packets_t* _localVar_4f2a;
	Packets_t* _context_4f2b;
	Packets_t* _triggerVar_4f2c;
	Packets_t* _function_4f2d;
	Packets_t _chart_4f2e;
	Packets_t _argDeclBase_4f37;
	Packets_t _dT_4f40;
	Packets_t _localVar_4f49;
	Packets_t _context_4f52;
	Packets_t _function_4f5b;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem chart_4f68;
		SFC::ArgDeclBase argDeclBase_4f69;
		SFC::DT dT_4f6a;
		SFC::LocalVar localVar_4f6b;
		SFC::Arg context_4f6c;
		SFC::Function function_4f6d;
	};

	std::list< Match> _matches;
};

class EdgeTest_4f81
{
public:
	void operator()( const Packets_t& events_4f82, const Packets_t& argdecls_4f84, const Packets_t& oldvals_4f86, const Packets_t& contexts_4f88, const Packets_t& initexprs_4f8a, Packets_t& argdecls_4f8c, Packets_t& oldvals_4f8d, Packets_t& contexts_4f8e, Packets_t& initexprs_4f8f, Packets_t& argdecls_4f90, Packets_t& oldvals_4f91, Packets_t& contexts_4f92, Packets_t& initexprs_4f93, Packets_t& argdecls_4f94, Packets_t& oldvals_4f95, Packets_t& contexts_4f96, Packets_t& initexprs_4f97);

protected:
	void executeOne( const Packets_t& events_4f82, const Packets_t& argdecls_4f84, const Packets_t& oldvals_4f86, const Packets_t& contexts_4f88, const Packets_t& initexprs_4f8a);
	bool isInputUnique( const Udm::Object& event_4fa6, const Udm::Object& argdecl_4fad, const Udm::Object& oldval_4fb4, const Udm::Object& context_4fbb, const Udm::Object& initexpr_4fc2);

private:
	Packets_t* _argdecl_4f98;
	Packets_t* _oldval_4f99;
	Packets_t* _context_4f9a;
	Packets_t* _initexpr_4f9b;
	Packets_t* _argdecl_4f9c;
	Packets_t* _oldval_4f9d;
	Packets_t* _context_4f9e;
	Packets_t* _initexpr_4f9f;
	Packets_t* _argdecl_4fa0;
	Packets_t* _oldval_4fa1;
	Packets_t* _context_4fa2;
	Packets_t* _initexpr_4fa3;
	Packets_t _event_4fa4;
	Packets_t _argdecl_4fab;
	Packets_t _oldval_4fb2;
	Packets_t _context_4fb9;
	Packets_t _initexpr_4fc0;
};

class RisingEdge_4fc7
{
public:
	bool operator()( const Packets_t& events_4fc8, const Packets_t& argDeclBases_4fca, const Packets_t& localVars_4fcd, const Packets_t& contexts_4fd0, const Packets_t& unaryExprss_4fd3, Packets_t& argDeclBases_4fcc, Packets_t& localVars_4fcf, Packets_t& contexts_4fd2, Packets_t& unaryExprss_4fd5);

protected:
	bool isInputUnique( const Udm::Object& event_4fde, const Udm::Object& argDeclBase_4fe7, const Udm::Object& localVar_4ff0, const Udm::Object& context_4ff9, const Udm::Object& unaryExprs_5002);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::Arg& Context, CyberComposition::Simulink::Event& Event, SFC::LocalVar& LocalVar, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& events_4fc8, const Packets_t& argDeclBases_4fca, const Packets_t& localVars_4fcd, const Packets_t& contexts_4fd0, const Packets_t& unaryExprss_4fd3);
	bool patternMatcher( const Udm::Object& event_4fdc, const Udm::Object& argDeclBase_4fe5, const Udm::Object& localVar_4fee, const Udm::Object& context_4ff7, const Udm::Object& unaryExprs_5000);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_5015, const SFC::LocalVar& localVar_5017, const SFC::Arg& context_5019, const SFC::UnaryExprs& unaryExprs_501b);

private:
	Packets_t* _argDeclBase_4fd6;
	Packets_t* _localVar_4fd7;
	Packets_t* _context_4fd8;
	Packets_t* _unaryExprs_4fd9;
	Packets_t _event_4fda;
	Packets_t _argDeclBase_4fe3;
	Packets_t _localVar_4fec;
	Packets_t _context_4ff5;
	Packets_t _unaryExprs_4ffe;
	class Match
	{
	public:
		CyberComposition::Simulink::Event event_500b;
		SFC::ArgDeclBase argDeclBase_500c;
		SFC::LocalVar localVar_500d;
		SFC::Arg context_500e;
		SFC::UnaryExprs unaryExprs_500f;
	};

	std::list< Match> _matches;
};

class FallingEdge_501d
{
public:
	bool operator()( const Packets_t& events_501e, const Packets_t& argDeclBases_5020, const Packets_t& localVars_5023, const Packets_t& contexts_5026, const Packets_t& unaryExprss_5029, Packets_t& argDeclBases_5022, Packets_t& localVars_5025, Packets_t& contexts_5028, Packets_t& unaryExprss_502b);

protected:
	bool isInputUnique( const Udm::Object& event_5034, const Udm::Object& argDeclBase_503d, const Udm::Object& localVar_5046, const Udm::Object& context_504f, const Udm::Object& unaryExprs_5058);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::Arg& Context, CyberComposition::Simulink::Event& Event, SFC::LocalVar& LocalVar, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& events_501e, const Packets_t& argDeclBases_5020, const Packets_t& localVars_5023, const Packets_t& contexts_5026, const Packets_t& unaryExprss_5029);
	bool patternMatcher( const Udm::Object& event_5032, const Udm::Object& argDeclBase_503b, const Udm::Object& localVar_5044, const Udm::Object& context_504d, const Udm::Object& unaryExprs_5056);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_506b, const SFC::LocalVar& localVar_506d, const SFC::Arg& context_506f, const SFC::UnaryExprs& unaryExprs_5071);

private:
	Packets_t* _argDeclBase_502c;
	Packets_t* _localVar_502d;
	Packets_t* _context_502e;
	Packets_t* _unaryExprs_502f;
	Packets_t _event_5030;
	Packets_t _argDeclBase_5039;
	Packets_t _localVar_5042;
	Packets_t _context_504b;
	Packets_t _unaryExprs_5054;
	class Match
	{
	public:
		CyberComposition::Simulink::Event event_5061;
		SFC::ArgDeclBase argDeclBase_5062;
		SFC::LocalVar localVar_5063;
		SFC::Arg context_5064;
		SFC::UnaryExprs unaryExprs_5065;
	};

	std::list< Match> _matches;
};

class EitherEdge_5073
{
public:
	bool operator()( const Packets_t& events_5074, const Packets_t& argDeclBases_5076, const Packets_t& localVars_5079, const Packets_t& contexts_507c, const Packets_t& unaryExprss_507f, Packets_t& argDeclBases_5078, Packets_t& localVars_507b, Packets_t& contexts_507e, Packets_t& unaryExprss_5081);

protected:
	bool isInputUnique( const Udm::Object& event_508a, const Udm::Object& argDeclBase_5093, const Udm::Object& localVar_509c, const Udm::Object& context_50a5, const Udm::Object& unaryExprs_50ae);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::Arg& Context, CyberComposition::Simulink::Event& Event, SFC::LocalVar& LocalVar, SFC::UnaryExprs& UnaryExprs);
	void processInputPackets( const Packets_t& events_5074, const Packets_t& argDeclBases_5076, const Packets_t& localVars_5079, const Packets_t& contexts_507c, const Packets_t& unaryExprss_507f);
	bool patternMatcher( const Udm::Object& event_5088, const Udm::Object& argDeclBase_5091, const Udm::Object& localVar_509a, const Udm::Object& context_50a3, const Udm::Object& unaryExprs_50ac);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_50c1, const SFC::LocalVar& localVar_50c3, const SFC::Arg& context_50c5, const SFC::UnaryExprs& unaryExprs_50c7);

private:
	Packets_t* _argDeclBase_5082;
	Packets_t* _localVar_5083;
	Packets_t* _context_5084;
	Packets_t* _unaryExprs_5085;
	Packets_t _event_5086;
	Packets_t _argDeclBase_508f;
	Packets_t _localVar_5098;
	Packets_t _context_50a1;
	Packets_t _unaryExprs_50aa;
	class Match
	{
	public:
		CyberComposition::Simulink::Event event_50b7;
		SFC::ArgDeclBase argDeclBase_50b8;
		SFC::LocalVar localVar_50b9;
		SFC::Arg context_50ba;
		SFC::UnaryExprs unaryExprs_50bb;
	};

	std::list< Match> _matches;
};

class FallingEdge_50c9
{
public:
	void operator()( const Packets_t& argDeclBases_50ca, const Packets_t& localVars_50cc, const Packets_t& contexts_50ce, const Packets_t& unaryExprss_50d0);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_50d6, const Udm::Object& localVar_50df, const Udm::Object& context_50e8, const Udm::Object& unaryExprs_50f1);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_50ca, const Packets_t& localVars_50cc, const Packets_t& contexts_50ce, const Packets_t& unaryExprss_50d0);
	bool patternMatcher( const Udm::Object& argDeclBase_50d4, const Udm::Object& localVar_50dd, const Udm::Object& context_50e6, const Udm::Object& unaryExprs_50ef);
	void effector();

private:
	Packets_t _argDeclBase_50d2;
	Packets_t _localVar_50db;
	Packets_t _context_50e4;
	Packets_t _unaryExprs_50ed;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_50fa;
		SFC::LocalVar localVar_50fb;
		SFC::Arg context_50fc;
		SFC::UnaryExprs unaryExprs_50fd;
	};

	std::list< Match> _matches;
};

class EitherEdge_5107
{
public:
	void operator()( const Packets_t& argDeclBases_5108, const Packets_t& localVars_510b, const Packets_t& contexts_510e, const Packets_t& unaryExprss_5111, Packets_t& argDeclBases_510a, Packets_t& localVars_510d, Packets_t& contexts_5110, Packets_t& leftUnaryExprss_5113, Packets_t& rightUnaryExprss_5114);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_511e, const Udm::Object& localVar_5127, const Udm::Object& context_5130, const Udm::Object& unaryExprs_5139);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_5108, const Packets_t& localVars_510b, const Packets_t& contexts_510e, const Packets_t& unaryExprss_5111);
	bool patternMatcher( const Udm::Object& argDeclBase_511c, const Udm::Object& localVar_5125, const Udm::Object& context_512e, const Udm::Object& unaryExprs_5137);
	void effector();
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_5149, const SFC::LocalVar& localVar_514b, const SFC::Arg& context_514d, const SFC::UnaryExprs& leftUnaryExprs_514f, const SFC::UnaryExprs& rightUnaryExprs_5151);

private:
	Packets_t* _argDeclBase_5115;
	Packets_t* _localVar_5116;
	Packets_t* _context_5117;
	Packets_t* _leftUnaryExprs_5118;
	Packets_t* _rightUnaryExprs_5119;
	Packets_t _argDeclBase_511a;
	Packets_t _localVar_5123;
	Packets_t _context_512c;
	Packets_t _unaryExprs_5135;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_5142;
		SFC::LocalVar localVar_5143;
		SFC::Arg context_5144;
		SFC::UnaryExprs unaryExprs_5145;
	};

	std::list< Match> _matches;
};

class StartAssignment_5153
{
public:
	void operator()( const Packets_t& charts_5155, const Packets_t& argDeclBases_5157, const Packets_t& oldvals_515a, const Packets_t& contexts_515d, const Packets_t& triggerVars_5160, const Packets_t& functions_5162, Packets_t& events_5154, Packets_t& argDeclBases_5159, Packets_t& oldvals_515c, Packets_t& contexts_515f, Packets_t& unaryExprss_5164);

protected:
	bool isInputUnique( const Udm::Object& chart_516e, const Udm::Object& argDeclBase_5177, const Udm::Object& oldval_5180, const Udm::Object& context_5189, const Udm::Object& triggerVar_5192, const Udm::Object& function_519b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, CyberComposition::Simulink::Subsystem& Chart, SFC::Arg& Context, CyberComposition::Simulink::Event& Event, SFC::Function& Function, CyberComposition::Simulink::Primitive& Primitive, CyberComposition::Simulink::State& State, SFC::LocalVar& TriggerVar, SFC::LocalVar& oldval);
	void processInputPackets( const Packets_t& charts_5155, const Packets_t& argDeclBases_5157, const Packets_t& oldvals_515a, const Packets_t& contexts_515d, const Packets_t& triggerVars_5160, const Packets_t& functions_5162);
	bool patternMatcher( const Udm::Object& chart_516c, const Udm::Object& argDeclBase_5175, const Udm::Object& oldval_517e, const Udm::Object& context_5187, const Udm::Object& triggerVar_5190, const Udm::Object& function_5199);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Event& event_51c3, const SFC::ArgDeclBase& argDeclBase_51c5, const SFC::LocalVar& oldval_51c7, const SFC::Arg& context_51c9, const SFC::UnaryExprs& unaryExprs_51cb);

private:
	Packets_t* _event_5165;
	Packets_t* _argDeclBase_5166;
	Packets_t* _oldval_5167;
	Packets_t* _context_5168;
	Packets_t* _unaryExprs_5169;
	Packets_t _chart_516a;
	Packets_t _argDeclBase_5173;
	Packets_t _oldval_517c;
	Packets_t _context_5185;
	Packets_t _triggerVar_518e;
	Packets_t _function_5197;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem chart_51ad;
		SFC::ArgDeclBase argDeclBase_51ae;
		SFC::LocalVar oldval_51af;
		SFC::Arg context_51b0;
		SFC::LocalVar triggerVar_51b1;
		SFC::Function function_51b2;
		CyberComposition::Simulink::State state_51b3;
		CyberComposition::Simulink::Event event_51b4;
		CyberComposition::Simulink::Primitive primitive_51b5;
	};

	std::list< Match> _matches;
};

class GetState_523a
{
public:
	void operator()( const Packets_t& subsystems_523b, const Packets_t& charts_523e, const Packets_t& mains_5242, const Packets_t& functionCalls_5245, Packets_t& subsystems_523d, Packets_t& charts_5240, Packets_t& states_5241, Packets_t& mains_5244, Packets_t& functionCalls_5247);

protected:
	bool isInputUnique( const Udm::Object& subsystem_5251, const Udm::Object& chart_525a, const Udm::Object& main_5263, const Udm::Object& functionCall_526c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_523b, const Packets_t& charts_523e, const Packets_t& mains_5242, const Packets_t& functionCalls_5245);
	bool patternMatcher( const Udm::Object& subsystem_524f, const Udm::Object& chart_5258, const Udm::Object& main_5261, const Udm::Object& functionCall_526a);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_527d, const CyberComposition::Simulink::Subsystem& chart_527f, const CyberComposition::Simulink::SFState& state_5281, const SFC::Function& main_5283, const SFC::FunctionCall& functionCall_5285);

private:
	Packets_t* _subsystem_5248;
	Packets_t* _chart_5249;
	Packets_t* _state_524a;
	Packets_t* _main_524b;
	Packets_t* _functionCall_524c;
	Packets_t _subsystem_524d;
	Packets_t _chart_5256;
	Packets_t _main_525f;
	Packets_t _functionCall_5268;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_5278;
		CyberComposition::Simulink::Subsystem chart_5279;
		SFC::Function main_527a;
		SFC::FunctionCall functionCall_527b;
		CyberComposition::Simulink::SFState state_527c;
	};

	std::list< Match> _matches;
};

class GetBlocks_52ee
{
public:
	void operator()( const Packets_t& comptypes_52ef, const Packets_t& projects_52f1, Packets_t& subsystems_52f3, Packets_t& projects_52f4);

protected:
	void callGetSubSystems_5320( const Packets_t& dataflows_52f8, const Packets_t& projects_52fb);

private:
	Packets_t* _subsystem_52f5;
	Packets_t* _project_52f6;
};

class GetSubSystems_52f7
{
public:
	void operator()( const Packets_t& dataflows_52f8, const Packets_t& projects_52fb, Packets_t& subsystems_52fa, Packets_t& projects_52fd);

protected:
	bool isInputUnique( const Udm::Object& dataflow_5304, const Udm::Object& project_530d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& dataflows_52f8, const Packets_t& projects_52fb);
	bool patternMatcher( const Udm::Object& dataflow_5302, const Udm::Object& project_530b);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_531c, const SFC::Project& project_531e);

private:
	Packets_t* _subsystem_52fe;
	Packets_t* _project_52ff;
	Packets_t _dataflow_5300;
	Packets_t _project_5309;
	class Match
	{
	public:
		CyberComposition::SimulinkWrapper dataflow_5319;
		SFC::Project project_531a;
		CyberComposition::Simulink::Subsystem subsystem_531b;
	};

	std::list< Match> _matches;
};

class FinishClasses_5329
{
public:
	void operator()( const Packets_t& dataflows_532a, const Packets_t& projects_532c, Packets_t& dataflows_532e, Packets_t& projects_532f);

protected:
	void callGetClasses_557c( const Packets_t& projects_5333);
	void callAssignToOutArgs_557e( const Packets_t& classs_5536);
	void callFinalizeContext_5580( const Packets_t& classs_5459);
	void callAddContextArgs_5582( const Packets_t& classs_5350);

private:
	Packets_t* _dataflow_5330;
	Packets_t* _project_5331;
};

class GetClasses_5332
{
public:
	void operator()( const Packets_t& projects_5333, Packets_t& classs_5335);

protected:
	bool isInputUnique( const Udm::Object& project_533b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& projects_5333);
	bool patternMatcher( const Udm::Object& project_5339);
	void effector();
	void outputAppender( const SFC::Class& class_534d);

private:
	Packets_t* _class_5336;
	Packets_t _project_5337;
	class Match
	{
	public:
		SFC::Project project_534a;
		SFC::Program program_534b;
		SFC::Class class_534c;
	};

	std::list< Match> _matches;
};

class AddContextArgs_534f
{
public:
	void operator()( const Packets_t& classs_5350);

protected:
	void callGetAllMethodCalls_5450( const Packets_t& classs_53a1);
	void callGetContextArg_5452( const Packets_t& methodCalls_5380, const Packets_t& callerContextArgs_5383);
	void calladdArgValExprs_5455( const Packets_t& methodCalls_5353, const Packets_t& callerContextArgs_5355);
};

class AddArgValExprs_5352
{
public:
	void operator()( const Packets_t& methodCalls_5353, const Packets_t& callerContextArgs_5355);

protected:
	bool isInputUnique( const Udm::Object& methodCall_535b, const Udm::Object& callerContextArg_5364);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgVal& ArgVal, SFC::Arg& CallerContextArg, SFC::LocalVar& LocalVar, SFC::FunctionCall& MethodCall, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& methodCalls_5353, const Packets_t& callerContextArgs_5355);
	bool patternMatcher( const Udm::Object& methodCall_5359, const Udm::Object& callerContextArg_5362);
	void effector();

private:
	Packets_t _methodCall_5357;
	Packets_t _callerContextArg_5360;
	class Match
	{
	public:
		SFC::FunctionCall methodCall_5372;
		SFC::Arg callerContextArg_5373;
		CyberComposition::Simulink::Subsystem subsystem_5374;
		SFC::LocalVar localVar_5375;
		SFC::ArgVal argVal_5376;
	};

	std::list< Match> _matches;
};

class GetContextArg_537f
{
public:
	void operator()( const Packets_t& methodCalls_5380, const Packets_t& callerContextArgs_5383, Packets_t& methodCalls_5382, Packets_t& callerContextArgs_5385);

protected:
	bool isInputUnique( const Udm::Object& methodCall_538c, const Udm::Object& callerContextArg_5395);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& methodCalls_5380, const Packets_t& callerContextArgs_5383);
	bool patternMatcher( const Udm::Object& methodCall_538a, const Udm::Object& callerContextArg_5393);
	void effector();
	void forwardInputs();

private:
	Packets_t* _methodCall_5386;
	Packets_t* _callerContextArg_5387;
	Packets_t _methodCall_5388;
	Packets_t _callerContextArg_5391;
	class Match
	{
	public:
		SFC::FunctionCall methodCall_539e;
		SFC::Arg callerContextArg_539f;
	};

	std::list< Match> _matches;
};

class GetAllMethodCalls_53a0
{
public:
	void operator()( const Packets_t& classs_53a1, Packets_t& methodcalls_53a3, Packets_t& contextargs_53a4, Packets_t& callerstructs_53a5);

protected:
	void callGetClassFunctions_5446( const Packets_t& callerClasss_541d);
	void callGetSubCompounds_5448( const Packets_t& compoundStatements_53e3, const Packets_t& args_53e6, const Packets_t& callerStructs_53e9);
	void callGetMethodCalls_544c( const Packets_t& compoundStatements_53ab, const Packets_t& contextargs_53ad, const Packets_t& callerStructs_53b0);

private:
	Packets_t* _methodcall_53a6;
	Packets_t* _contextarg_53a7;
	Packets_t* _callerstruct_53a8;
};

class GetMethodCalls_53a9
{
public:
	void operator()( const Packets_t& compoundStatements_53ab, const Packets_t& contextargs_53ad, const Packets_t& callerStructs_53b0, Packets_t& methodCalls_53aa, Packets_t& contextargs_53af, Packets_t& callerStructs_53b2);

protected:
	bool isInputUnique( const Udm::Object& compoundStatement_53ba, const Udm::Object& contextarg_53c3, const Udm::Object& callerStruct_53cc);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& compoundStatements_53ab, const Packets_t& contextargs_53ad, const Packets_t& callerStructs_53b0);
	bool patternMatcher( const Udm::Object& compoundStatement_53b8, const Udm::Object& contextarg_53c1, const Udm::Object& callerStruct_53ca);
	void effector();
	void outputAppender( const SFC::FunctionCall& methodCall_53dc, const SFC::Arg& contextarg_53de, const SFC::Struct& callerStruct_53e0);

private:
	Packets_t* _methodCall_53b3;
	Packets_t* _contextarg_53b4;
	Packets_t* _callerStruct_53b5;
	Packets_t _compoundStatement_53b6;
	Packets_t _contextarg_53bf;
	Packets_t _callerStruct_53c8;
	class Match
	{
	public:
		SFC::CompoundStatement compoundStatement_53d8;
		SFC::Arg contextarg_53d9;
		SFC::Struct callerStruct_53da;
		SFC::FunctionCall methodCall_53db;
	};

	std::list< Match> _matches;
};

class GetSubCompounds_53e2
{
public:
	void operator()( const Packets_t& compoundStatements_53e3, const Packets_t& args_53e6, const Packets_t& callerStructs_53e9, Packets_t& subCompounds_53e5, Packets_t& args_53e8, Packets_t& callerStructs_53eb);

protected:
	bool isInputUnique( const Udm::Object& compoundStatement_53f3, const Udm::Object& arg_53fc, const Udm::Object& callerStruct_5405);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& compoundStatements_53e3, const Packets_t& args_53e6, const Packets_t& callerStructs_53e9);
	bool patternMatcher( const Udm::Object& compoundStatement_53f1, const Udm::Object& arg_53fa, const Udm::Object& callerStruct_5403);
	void effector();
	void outputAppender( const SFC::CompoundStatement& subCompound_5415, const SFC::Arg& arg_5417, const SFC::Struct& callerStruct_5419);

private:
	Packets_t* _subCompound_53ec;
	Packets_t* _arg_53ed;
	Packets_t* _callerStruct_53ee;
	Packets_t _compoundStatement_53ef;
	Packets_t _arg_53f8;
	Packets_t _callerStruct_5401;
	class Match
	{
	public:
		SFC::CompoundStatement compoundStatement_5411;
		SFC::Arg arg_5412;
		SFC::Struct callerStruct_5413;
		SFC::CompoundStatement subCompound_5414;
	};

	std::list< Match> _matches;
};

class GetClassFunctions_541b
{
public:
	void operator()( const Packets_t& callerClasss_541d, Packets_t& callerMethods_541c, Packets_t& args_541f, Packets_t& callerStructs_5420);

protected:
	bool isInputUnique( const Udm::Object& callerClass_5428);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& callerClasss_541d);
	bool patternMatcher( const Udm::Object& callerClass_5426);
	void effector();
	void outputAppender( const SFC::Function& callerMethod_5440, const SFC::Arg& arg_5442, const SFC::Struct& callerStruct_5444);

private:
	Packets_t* _callerMethod_5421;
	Packets_t* _arg_5422;
	Packets_t* _callerStruct_5423;
	Packets_t _callerClass_5424;
	class Match
	{
	public:
		SFC::Class callerClass_543c;
		SFC::Function callerMethod_543d;
		SFC::Arg arg_543e;
		SFC::Struct callerStruct_543f;
	};

	std::list< Match> _matches;
};

class FinalizeContext_5458
{
public:
	void operator()( const Packets_t& classs_5459, Packets_t& classs_545b);

protected:
	void callGetAllMethodCalls_552d( const Packets_t& classs_53a1);
	void callGetCalleeObject_552f( const Packets_t& methodCalls_5507, const Packets_t& callerStructs_550a);
	void callAddMembers_5532( const Packets_t& calleeObjects_545e, const Packets_t& callerStructs_5460);

private:
	Packets_t* _class_545c;
};

class AddMembers_545d
{
public:
	void operator()( const Packets_t& calleeObjects_545e, const Packets_t& callerStructs_5460);

protected:
	void executeOne( const Packets_t& calleeObjects_545e, const Packets_t& callerStructs_5460);
	bool isInputUnique( const Udm::Object& calleeObject_5464, const Udm::Object& callerStruct_546b);
	void callContextTest_5500( const Packets_t& calleeObjects_5471, const Packets_t& callerStructs_5473);
	void callAddContextMember_5503( const Packets_t& calleeSubsystems_54dd, const Packets_t& callerStructs_54df);

private:
	Packets_t _calleeObject_5462;
	Packets_t _callerStruct_5469;
};

class ContextTest_5470
{
public:
	void operator()( const Packets_t& calleeObjects_5471, const Packets_t& callerStructs_5473, Packets_t& calleeObjects_5475, Packets_t& callerStructs_5476, Packets_t& calleeObjects_5477, Packets_t& callerStructs_5478);

protected:
	void executeOne( const Packets_t& calleeObjects_5471, const Packets_t& callerStructs_5473);
	bool isInputUnique( const Udm::Object& calleeObject_547f, const Udm::Object& callerStruct_5486);

private:
	Packets_t* _calleeObject_5479;
	Packets_t* _callerStruct_547a;
	Packets_t* _calleeObject_547b;
	Packets_t* _callerStruct_547c;
	Packets_t _calleeObject_547d;
	Packets_t _callerStruct_5484;
};

class CallerStructHasMember_548b
{
public:
	bool operator()( const Packets_t& calleeSubsystems_548c, const Packets_t& callerStructs_548f, Packets_t& calleeSubsystems_548e, Packets_t& callerStructs_5491);

protected:
	bool isInputUnique( const Udm::Object& calleeSubsystem_5498, const Udm::Object& callerStruct_54a1);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& calleeSubsystems_548c, const Packets_t& callerStructs_548f);
	bool patternMatcher( const Udm::Object& calleeSubsystem_5496, const Udm::Object& callerStruct_549f);
	void outputAppender( const CyberComposition::Simulink::Subsystem& calleeSubsystem_54b3, const SFC::Struct& callerStruct_54b5);

private:
	Packets_t* _calleeSubsystem_5492;
	Packets_t* _callerStruct_5493;
	Packets_t _calleeSubsystem_5494;
	Packets_t _callerStruct_549d;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem calleeSubsystem_54b0;
		SFC::Struct callerStruct_54b1;
		SFC::LocalVar localVar_54b2;
	};

	std::list< Match> _matches;
};

class Otherwise_54b7
{
public:
	bool operator()( const Packets_t& calleeSubsystems_54b8, const Packets_t& callerStructs_54bb, Packets_t& calleeSubsystems_54ba, Packets_t& callerStructs_54bd);

protected:
	bool isInputUnique( const Udm::Object& calleeSubsystem_54c4, const Udm::Object& callerStruct_54cd);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& calleeSubsystems_54b8, const Packets_t& callerStructs_54bb);
	bool patternMatcher( const Udm::Object& calleeSubsystem_54c2, const Udm::Object& callerStruct_54cb);
	void outputAppender( const CyberComposition::Simulink::Subsystem& calleeSubsystem_54d8, const SFC::Struct& callerStruct_54da);

private:
	Packets_t* _calleeSubsystem_54be;
	Packets_t* _callerStruct_54bf;
	Packets_t _calleeSubsystem_54c0;
	Packets_t _callerStruct_54c9;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem calleeSubsystem_54d6;
		SFC::Struct callerStruct_54d7;
	};

	std::list< Match> _matches;
};

class AddContextMember_54dc
{
public:
	void operator()( const Packets_t& calleeSubsystems_54dd, const Packets_t& callerStructs_54df);

protected:
	bool isInputUnique( const Udm::Object& calleeSubsystem_54e5, const Udm::Object& callerStruct_54ee);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& calleeSubsystems_54dd, const Packets_t& callerStructs_54df);
	bool patternMatcher( const Udm::Object& calleeSubsystem_54e3, const Udm::Object& callerStruct_54ec);
	void effector();

private:
	Packets_t _calleeSubsystem_54e1;
	Packets_t _callerStruct_54ea;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem calleeSubsystem_54fb;
		SFC::Struct callerStruct_54fc;
		SFC::Class calleeClass_54fd;
		SFC::Struct calleeStruct_54fe;
	};

	std::list< Match> _matches;
};

class GetCalleeObject_5506
{
public:
	void operator()( const Packets_t& methodCalls_5507, const Packets_t& callerStructs_550a, Packets_t& calleeSubsystems_5509, Packets_t& callerStructs_550c);

protected:
	bool isInputUnique( const Udm::Object& methodCall_5513, const Udm::Object& callerStruct_551c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& methodCalls_5507, const Packets_t& callerStructs_550a);
	bool patternMatcher( const Udm::Object& methodCall_5511, const Udm::Object& callerStruct_551a);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& calleeSubsystem_5529, const SFC::Struct& callerStruct_552b);

private:
	Packets_t* _calleeSubsystem_550d;
	Packets_t* _callerStruct_550e;
	Packets_t _methodCall_550f;
	Packets_t _callerStruct_5518;
	class Match
	{
	public:
		SFC::FunctionCall methodCall_5526;
		SFC::Struct callerStruct_5527;
		CyberComposition::Simulink::Subsystem calleeSubsystem_5528;
	};

	std::list< Match> _matches;
};

class AssignToOutArgs_5535
{
public:
	void operator()( const Packets_t& classs_5536, Packets_t& classs_5538);

protected:
	void callIdenticalOutArgs_557a( const Packets_t& classs_553b);

private:
	Packets_t* _class_5539;
};

class IdenticalOutArgs_553a
{
public:
	void operator()( const Packets_t& classs_553b);

protected:
	bool isInputUnique( const Udm::Object& class_5541);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::Block& Block, SFC::Class& Class, SFC::Function& Function, SFC::Arg& LeftArg, CyberComposition::Simulink::OutPort& LeftSubsystemOutPort, CyberComposition::Simulink::Line& Line, CyberComposition::Simulink::OutPort& OutPort, SFC::Arg& RightArg, CyberComposition::Simulink::OutPort& RightSubsystemOutPort, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& classs_553b);
	bool patternMatcher( const Udm::Object& class_553f);
	void effector();

private:
	Packets_t _class_553d;
	class Match
	{
	public:
		SFC::Class class_5566;
		CyberComposition::Simulink::Block block_5567;
		CyberComposition::Simulink::Subsystem subsystem_5568;
		CyberComposition::Simulink::OutPort rightSubsystemOutPort_5569;
		SFC::Arg rightArg_556a;
		CyberComposition::Simulink::OutPort outPort_556b;
		CyberComposition::Simulink::Line line_556c;
		CyberComposition::Simulink::OutPort leftSubsystemOutPort_556d;
		SFC::Arg leftArg_556e;
		SFC::Function function_556f;
	};

	std::list< Match> _matches;
};

class CheckPorts_5584
{
public:
	void operator()( const Packets_t& comptypes_5585, const Packets_t& projects_5587, Packets_t& dataflows_5589, Packets_t& projects_558a);

protected:
	void callCheckPortTypes_5771( const Packets_t& dataflows_5631, const Packets_t& projects_5633);
	void callCheckError_5774( const Packets_t& dataflows_558e, const Packets_t& projects_5590);

private:
	Packets_t* _dataflow_558b;
	Packets_t* _project_558c;
};

class CheckError_558d
{
public:
	void operator()( const Packets_t& dataflows_558e, const Packets_t& projects_5590, Packets_t& dataflows_5592, Packets_t& projects_5593);

protected:
	void executeOne( const Packets_t& dataflows_558e, const Packets_t& projects_5590);
	bool isInputUnique( const Udm::Object& dataflow_5598, const Udm::Object& project_559f);
	void callCheckError_562a( const Packets_t& dataflows_55c2, const Packets_t& projects_55c4);
	void callReportError_562d( const Packets_t& dataflows_55a5, const Packets_t& projects_55a7);

private:
	Packets_t* _dataflow_5594;
	Packets_t* _project_5595;
	Packets_t _dataflow_5596;
	Packets_t _project_559d;
};

class ReportError_55a4
{
public:
	void operator()( const Packets_t& dataflows_55a5, const Packets_t& projects_55a7);

protected:
	bool isInputUnique( const Udm::Object& dataflow_55ad, const Udm::Object& project_55b6);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& dataflows_55a5, const Packets_t& projects_55a7);
	bool patternMatcher( const Udm::Object& dataflow_55ab, const Udm::Object& project_55b4);
	void effector();

private:
	Packets_t _dataflow_55a9;
	Packets_t _project_55b2;
	class Match
	{
	public:
		CyberComposition::SimulinkWrapper dataflow_55bf;
		SFC::Project project_55c0;
	};

	std::list< Match> _matches;
};

class CheckError_55c1
{
public:
	void operator()( const Packets_t& dataflows_55c2, const Packets_t& projects_55c4, Packets_t& dataflows_55c6, Packets_t& projects_55c7, Packets_t& dataflows_55c8, Packets_t& projects_55c9);

protected:
	void executeOne( const Packets_t& dataflows_55c2, const Packets_t& projects_55c4);
	bool isInputUnique( const Udm::Object& dataflow_55d0, const Udm::Object& project_55d7);

private:
	Packets_t* _dataflow_55ca;
	Packets_t* _project_55cb;
	Packets_t* _dataflow_55cc;
	Packets_t* _project_55cd;
	Packets_t _dataflow_55ce;
	Packets_t _project_55d5;
};

class NoError_55dc
{
public:
	bool operator()( const Packets_t& dataflows_55dd, const Packets_t& projects_55e0, Packets_t& dataflows_55df, Packets_t& projects_55e2);

protected:
	bool isInputUnique( const Udm::Object& dataflow_55e9, const Udm::Object& project_55f2);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::SimulinkWrapper& Dataflow, SFC::Project& Project);
	void processInputPackets( const Packets_t& dataflows_55dd, const Packets_t& projects_55e0);
	bool patternMatcher( const Udm::Object& dataflow_55e7, const Udm::Object& project_55f0);
	void outputAppender( const CyberComposition::SimulinkWrapper& dataflow_55ff, const SFC::Project& project_5601);

private:
	Packets_t* _dataflow_55e3;
	Packets_t* _project_55e4;
	Packets_t _dataflow_55e5;
	Packets_t _project_55ee;
	class Match
	{
	public:
		CyberComposition::SimulinkWrapper dataflow_55fb;
		SFC::Project project_55fc;
	};

	std::list< Match> _matches;
};

class NoErrorReported_5603
{
public:
	bool operator()( const Packets_t& dataflows_5604, const Packets_t& projects_5607, Packets_t& dataflows_5606, Packets_t& projects_5609);

protected:
	bool isInputUnique( const Udm::Object& dataflow_5610, const Udm::Object& project_5619);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::SimulinkWrapper& Dataflow, SFC::Project& Project);
	void processInputPackets( const Packets_t& dataflows_5604, const Packets_t& projects_5607);
	bool patternMatcher( const Udm::Object& dataflow_560e, const Udm::Object& project_5617);
	void outputAppender( const CyberComposition::SimulinkWrapper& dataflow_5626, const SFC::Project& project_5628);

private:
	Packets_t* _dataflow_560a;
	Packets_t* _project_560b;
	Packets_t _dataflow_560c;
	Packets_t _project_5615;
	class Match
	{
	public:
		CyberComposition::SimulinkWrapper dataflow_5622;
		SFC::Project project_5623;
	};

	std::list< Match> _matches;
};

class CheckPortTypes_5630
{
public:
	void operator()( const Packets_t& dataflows_5631, const Packets_t& projects_5633, Packets_t& dataflows_5635, Packets_t& projects_5636);

protected:
	void callGetSubsystems_576d( const Packets_t& comptypes_115a);
	void callCheckAllPorts_576f( const Packets_t& blocks_563a);

private:
	Packets_t* _dataflow_5637;
	Packets_t* _project_5638;
};

class CheckAllPorts_5639
{
public:
	void operator()( const Packets_t& blocks_563a);

protected:
	void callFilterInstances_5765( const Packets_t& blocks_563d);
	void callCheckPorts_5767( const Packets_t& blocks_5666);
	void callChartFilter_5769( const Packets_t& blocks_570d);
	void callGetSubBlocks_576b( const Packets_t& subsystems_574e);
};

class FilterInstances_563c
{
public:
	void operator()( const Packets_t& blocks_563d, Packets_t& blocks_563f);

protected:
	void callNotAnInstanceTest_5663( const Packets_t& blocks_5642);

private:
	Packets_t* _block_5640;
};

class NotAnInstanceTest_5641
{
public:
	void operator()( const Packets_t& blocks_5642, Packets_t& blocks_5644);

protected:
	void executeOne( const Packets_t& blocks_5642);
	bool isInputUnique( const Udm::Object& block_5648);

private:
	Packets_t* _block_5645;
	Packets_t _block_5646;
};

class NotAnInstance_564d
{
public:
	bool operator()( const Packets_t& blocks_564e, Packets_t& blocks_5650);

protected:
	bool isInputUnique( const Udm::Object& block_5656);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::Block& Block);
	void processInputPackets( const Packets_t& blocks_564e);
	bool patternMatcher( const Udm::Object& block_5654);
	void outputAppender( const CyberComposition::Simulink::Block& block_5661);

private:
	Packets_t* _block_5651;
	Packets_t _block_5652;
	class Match
	{
	public:
		CyberComposition::Simulink::Block block_565f;
	};

	std::list< Match> _matches;
};

class CheckPorts_5665
{
public:
	void operator()( const Packets_t& blocks_5666, Packets_t& blocks_5668);

protected:
	void callGetPorts_5705( const Packets_t& blocks_56d6);
	void callCheckPort_5707( const Packets_t& blocks_566b, const Packets_t& ports_566d);
	void callSetError_570a( const Packets_t& blocks_56f3);

private:
	Packets_t* _block_5669;
};

class CheckPort_566a
{
public:
	void operator()( const Packets_t& blocks_566b, const Packets_t& ports_566d, Packets_t& blocks_566f, Packets_t& ports_5670, Packets_t& blocks_5671, Packets_t& ports_5672);

protected:
	void executeOne( const Packets_t& blocks_566b, const Packets_t& ports_566d);
	bool isInputUnique( const Udm::Object& block_5679, const Udm::Object& port_5680);

private:
	Packets_t* _block_5673;
	Packets_t* _port_5674;
	Packets_t* _block_5675;
	Packets_t* _port_5676;
	Packets_t _block_5677;
	Packets_t _port_567e;
};

class PortHasType_5685
{
public:
	bool operator()( const Packets_t& blocks_5686, const Packets_t& ports_5689, Packets_t& blocks_5688, Packets_t& ports_568b);

protected:
	bool isInputUnique( const Udm::Object& block_5692, const Udm::Object& port_569b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& blocks_5686, const Packets_t& ports_5689);
	bool patternMatcher( const Udm::Object& block_5690, const Udm::Object& port_5699);
	void outputAppender( const CyberComposition::Simulink::Block& block_56ac, const CyberComposition::Simulink::SF_Port& port_56ae);

private:
	Packets_t* _block_568c;
	Packets_t* _port_568d;
	Packets_t _block_568e;
	Packets_t _port_5697;
	class Match
	{
	public:
		CyberComposition::Simulink::Block block_56a8;
		CyberComposition::Simulink::SF_Port port_56a9;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_56aa;
		CyberComposition::Simulink::SF_TypeBase typeBase_56ab;
	};

	std::list< Match> _matches;
};

class Otherwise_56b0
{
public:
	bool operator()( const Packets_t& blocks_56b1, const Packets_t& ports_56b4, Packets_t& blocks_56b3, Packets_t& ports_56b6);

protected:
	bool isInputUnique( const Udm::Object& block_56bd, const Udm::Object& port_56c6);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& blocks_56b1, const Packets_t& ports_56b4);
	bool patternMatcher( const Udm::Object& block_56bb, const Udm::Object& port_56c4);
	void outputAppender( const CyberComposition::Simulink::Block& block_56d1, const CyberComposition::Simulink::SF_Port& port_56d3);

private:
	Packets_t* _block_56b7;
	Packets_t* _port_56b8;
	Packets_t _block_56b9;
	Packets_t _port_56c2;
	class Match
	{
	public:
		CyberComposition::Simulink::Block block_56cf;
		CyberComposition::Simulink::SF_Port port_56d0;
	};

	std::list< Match> _matches;
};

class GetPorts_56d5
{
public:
	void operator()( const Packets_t& blocks_56d6, Packets_t& blocks_56d8, Packets_t& ports_56d9);

protected:
	bool isInputUnique( const Udm::Object& block_56e0);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& blocks_56d6);
	bool patternMatcher( const Udm::Object& block_56de);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Block& block_56ee, const CyberComposition::Simulink::SF_Port& port_56f0);

private:
	Packets_t* _block_56da;
	Packets_t* _port_56db;
	Packets_t _block_56dc;
	class Match
	{
	public:
		CyberComposition::Simulink::Block block_56ec;
		CyberComposition::Simulink::SF_Port port_56ed;
	};

	std::list< Match> _matches;
};

class SetError_56f2
{
public:
	void operator()( const Packets_t& blocks_56f3, Packets_t& blocks_56f5);

protected:
	bool isInputUnique( const Udm::Object& block_56fb);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& blocks_56f3);
	bool patternMatcher( const Udm::Object& block_56f9);
	void effector();
	void forwardInputs();

private:
	Packets_t* _block_56f6;
	Packets_t _block_56f7;
	class Match
	{
	public:
		CyberComposition::Simulink::Block block_5704;
	};

	std::list< Match> _matches;
};

class ChartFilter_570c
{
public:
	void operator()( const Packets_t& blocks_570d, Packets_t& systems_570f, Packets_t& systems_5710);

protected:
	void executeOne( const Packets_t& blocks_570d);
	bool isInputUnique( const Udm::Object& block_5715);

private:
	Packets_t* _system_5711;
	Packets_t* _system_5712;
	Packets_t _block_5713;
};

class Block_571a
{
public:
	bool operator()( const Packets_t& subsystems_571b, Packets_t& subsystems_571d);

protected:
	bool isInputUnique( const Udm::Object& subsystem_5723);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_571b);
	bool patternMatcher( const Udm::Object& subsystem_5721);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_5735);

private:
	Packets_t* _subsystem_571e;
	Packets_t _subsystem_571f;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_5732;
		CyberComposition::Simulink::Primitive primitive_5733;
		CyberComposition::Simulink::State state_5734;
	};

	std::list< Match> _matches;
};

class IsSubsystem_5737
{
public:
	bool operator()( const Packets_t& subsystems_5738, Packets_t& subsystems_573a);

protected:
	bool isInputUnique( const Udm::Object& subsystem_5740);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_5738);
	bool patternMatcher( const Udm::Object& subsystem_573e);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_574a);

private:
	Packets_t* _subsystem_573b;
	Packets_t _subsystem_573c;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_5749;
	};

	std::list< Match> _matches;
};

class GetSubBlocks_574c
{
public:
	void operator()( const Packets_t& subsystems_574e, Packets_t& blocks_574d);

protected:
	bool isInputUnique( const Udm::Object& subsystem_5755);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_574e);
	bool patternMatcher( const Udm::Object& subsystem_5753);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Block& block_5763);

private:
	Packets_t* _block_5750;
	Packets_t _subsystem_5751;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_5761;
		CyberComposition::Simulink::Block block_5762;
	};

	std::list< Match> _matches;
};

class ParamInitFcn_5777
{
public:
	void operator()( const Packets_t& dataflows_5778, const Packets_t& projects_577a, Packets_t& dataflows_577c, Packets_t& projects_577d);

protected:
	void callCreateParamInitFcn_5a10( const Packets_t& sigflows_5781);

private:
	Packets_t* _dataflow_577e;
	Packets_t* _project_577f;
};

class CreateParamInitFcn_5780
{
public:
	void operator()( const Packets_t& sigflows_5781);

protected:
	void callCreateParamInitFcn_59f2( const Packets_t& signalFlowModels_5784);
	void callSortParamRefs_59f4( const Packets_t& signalFlowModels_59ba, const Packets_t& functions_59bd, const Packets_t& contexts_59c0);
	void callCreateAssignment_59f8( const Packets_t& parameterRefs_57ad, const Packets_t& functions_57b0, const Packets_t& contexts_57b3);
	void callTestContext_59fc( const Packets_t& localvars_583b, const Packets_t& structvars_583d, const Packets_t& exprss_583f, const Packets_t& ctxstrcts_5841);
	void callNextLocalVar_5a01( const Packets_t& localVars_57f2, const Packets_t& structVars_57f5, const Packets_t& dotExprss_57f8, const Packets_t& structs_57fb);
	void callNextDot_5a06( const Packets_t& localVars_5977, const Packets_t& structVars_597a, const Packets_t& dotExprss_597c, const Packets_t& structs_597f);
	void callLastDot_5a0b( const Packets_t& localVars_5944, const Packets_t& structVars_5946, const Packets_t& dotExprss_5948, const Packets_t& structs_594a);
};

class CreateParamInitFcn_5783
{
public:
	void operator()( const Packets_t& signalFlowModels_5784, Packets_t& signalFlowModels_5786, Packets_t& functions_5787, Packets_t& contexts_5788);

protected:
	bool isInputUnique( const Udm::Object& signalFlowModel_5790);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& signalFlowModels_5784);
	bool patternMatcher( const Udm::Object& signalFlowModel_578e);
	void effector();
	void outputAppender( const CyberComposition::SimulinkWrapper& signalFlowModel_57a6, const SFC::Function& function_57a8, const SFC::Arg& context_57aa);

private:
	Packets_t* _signalFlowModel_5789;
	Packets_t* _function_578a;
	Packets_t* _context_578b;
	Packets_t _signalFlowModel_578c;
	class Match
	{
	public:
		CyberComposition::SimulinkWrapper signalFlowModel_57a0;
		SFC::Class class_57a1;
		CyberComposition::Simulink::Subsystem subsystem_57a2;
		SFC::Struct struct_57a3;
	};

	std::list< Match> _matches;
};

class CreateAssignment_57ac
{
public:
	void operator()( const Packets_t& parameterRefs_57ad, const Packets_t& functions_57b0, const Packets_t& contexts_57b3, Packets_t& localVars_57af, Packets_t& dotExprss_57b2, Packets_t& structs_57b5);

protected:
	bool isInputUnique( const Udm::Object& parameterRef_57bd, const Udm::Object& function_57c6, const Udm::Object& context_57cf);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& parameterRefs_57ad, const Packets_t& functions_57b0, const Packets_t& contexts_57b3);
	bool patternMatcher( const Udm::Object& parameterRef_57bb, const Udm::Object& function_57c4, const Udm::Object& context_57cd);
	void effector();
	void outputAppender( const SFC::LocalVar& localVar_57eb, const SFC::BinaryExprs& dotExprs_57ed, const SFC::Struct& struct_57ef);

private:
	Packets_t* _localVar_57b6;
	Packets_t* _dotExprs_57b7;
	Packets_t* _struct_57b8;
	Packets_t _parameterRef_57b9;
	Packets_t _function_57c2;
	Packets_t _context_57cb;
	class Match
	{
	public:
		CyberComposition::ParameterRef parameterRef_57de;
		SFC::Function function_57df;
		SFC::Arg context_57e0;
		CyberComposition::Simulink::SF_Parameter sF_Parameter_57e1;
		SFC::LocalVar localVar_57e2;
		SFC::DT dT_57e3;
		SFC::Struct struct_57e4;
	};

	std::list< Match> _matches;
};

class NextLocalVar_57f1
{
public:
	void operator()( const Packets_t& localVars_57f2, const Packets_t& structVars_57f5, const Packets_t& dotExprss_57f8, const Packets_t& structs_57fb, Packets_t& localVars_57f4, Packets_t& nextStructVars_57f7, Packets_t& dotExprss_57fa, Packets_t& structs_57fd);

protected:
	bool isInputUnique( const Udm::Object& localVar_5806, const Udm::Object& structVar_580f, const Udm::Object& dotExprs_5818, const Udm::Object& struct_5821);
	void processInputPackets( const Packets_t& localVars_57f2, const Packets_t& structVars_57f5, const Packets_t& dotExprss_57f8, const Packets_t& structs_57fb);
	bool patternMatcher( const Udm::Object& localVar_5804, const Udm::Object& structVar_580d, const Udm::Object& dotExprs_5816, const Udm::Object& struct_581f);
	void effector();
	void outputAppender( const SFC::LocalVar& localVar_5832, const SFC::LocalVar& nextStructVar_5834, const SFC::BinaryExprs& dotExprs_5836, const SFC::Struct& struct_5838);

private:
	Packets_t* _localVar_57fe;
	Packets_t* _nextStructVar_57ff;
	Packets_t* _dotExprs_5800;
	Packets_t* _struct_5801;
	Packets_t _localVar_5802;
	Packets_t _structVar_580b;
	Packets_t _dotExprs_5814;
	Packets_t _struct_581d;
	class Match
	{
	public:
		SFC::LocalVar localVar_582c;
		SFC::LocalVar structVar_582d;
		SFC::BinaryExprs dotExprs_582e;
		SFC::Struct struct_582f;
		SFC::LocalVar nextStructVar_5830;
		SFC::Struct nextStruct_5831;
	};

	std::list< Match> _matches;
};

class TestContext_583a
{
public:
	void operator()( const Packets_t& localvars_583b, const Packets_t& structvars_583d, const Packets_t& exprss_583f, const Packets_t& ctxstrcts_5841, Packets_t& localVars_5843, Packets_t& structvars_5844, Packets_t& exprss_5845, Packets_t& ctxstrcts_5846, Packets_t& localVars_5847, Packets_t& structvars_5848, Packets_t& exprss_5849, Packets_t& ctxstrcts_584a, Packets_t& localVars_584b, Packets_t& structvars_584c, Packets_t& exprss_584d, Packets_t& ctxstrcts_584e);

protected:
	void executeOne( const Packets_t& localvars_583b, const Packets_t& structvars_583d, const Packets_t& exprss_583f, const Packets_t& ctxstrcts_5841);
	bool isInputUnique( const Udm::Object& localvar_585d, const Udm::Object& structvar_5864, const Udm::Object& exprs_586b, const Udm::Object& ctxstrct_5872);

private:
	Packets_t* _localVar_584f;
	Packets_t* _structvar_5850;
	Packets_t* _exprs_5851;
	Packets_t* _ctxstrct_5852;
	Packets_t* _localVar_5853;
	Packets_t* _structvar_5854;
	Packets_t* _exprs_5855;
	Packets_t* _ctxstrct_5856;
	Packets_t* _localVar_5857;
	Packets_t* _structvar_5858;
	Packets_t* _exprs_5859;
	Packets_t* _ctxstrct_585a;
	Packets_t _localvar_585b;
	Packets_t _structvar_5862;
	Packets_t _exprs_5869;
	Packets_t _ctxstrct_5870;
};

class LVinContext_5877
{
public:
	bool operator()( const Packets_t& localVars_5878, const Packets_t& structVars_587b, const Packets_t& binaryExprss_587e, const Packets_t& structs_5881, Packets_t& localVars_587a, Packets_t& structVars_587d, Packets_t& binaryExprss_5880, Packets_t& structs_5883);

protected:
	bool isInputUnique( const Udm::Object& localVar_588c, const Udm::Object& structVar_5895, const Udm::Object& binaryExprs_589e, const Udm::Object& struct_58a7);
	void processInputPackets( const Packets_t& localVars_5878, const Packets_t& structVars_587b, const Packets_t& binaryExprss_587e, const Packets_t& structs_5881);
	bool patternMatcher( const Udm::Object& localVar_588a, const Udm::Object& structVar_5893, const Udm::Object& binaryExprs_589c, const Udm::Object& struct_58a5);
	void outputAppender( const SFC::LocalVar& localVar_58b4, const SFC::LocalVar& structVar_58b6, const SFC::BinaryExprs& binaryExprs_58b8, const SFC::Struct& struct_58ba);

private:
	Packets_t* _localVar_5884;
	Packets_t* _structVar_5885;
	Packets_t* _binaryExprs_5886;
	Packets_t* _struct_5887;
	Packets_t _localVar_5888;
	Packets_t _structVar_5891;
	Packets_t _binaryExprs_589a;
	Packets_t _struct_58a3;
	class Match
	{
	public:
		SFC::LocalVar localVar_58b0;
		SFC::LocalVar structVar_58b1;
		SFC::BinaryExprs binaryExprs_58b2;
		SFC::Struct struct_58b3;
	};

	std::list< Match> _matches;
};

class SVinContext_58bc
{
public:
	bool operator()( const Packets_t& localVars_58bd, const Packets_t& structVars_58c0, const Packets_t& binaryExprss_58c3, const Packets_t& structs_58c6, Packets_t& localVars_58bf, Packets_t& structVars_58c2, Packets_t& binaryExprss_58c5, Packets_t& structs_58c8);

protected:
	bool isInputUnique( const Udm::Object& localVar_58d1, const Udm::Object& structVar_58da, const Udm::Object& binaryExprs_58e3, const Udm::Object& struct_58ec);
	void processInputPackets( const Packets_t& localVars_58bd, const Packets_t& structVars_58c0, const Packets_t& binaryExprss_58c3, const Packets_t& structs_58c6);
	bool patternMatcher( const Udm::Object& localVar_58cf, const Udm::Object& structVar_58d8, const Udm::Object& binaryExprs_58e1, const Udm::Object& struct_58ea);
	void outputAppender( const SFC::LocalVar& localVar_58f9, const SFC::LocalVar& structVar_58fb, const SFC::BinaryExprs& binaryExprs_58fd, const SFC::Struct& struct_58ff);

private:
	Packets_t* _localVar_58c9;
	Packets_t* _structVar_58ca;
	Packets_t* _binaryExprs_58cb;
	Packets_t* _struct_58cc;
	Packets_t _localVar_58cd;
	Packets_t _structVar_58d6;
	Packets_t _binaryExprs_58df;
	Packets_t _struct_58e8;
	class Match
	{
	public:
		SFC::LocalVar localVar_58f5;
		SFC::LocalVar structVar_58f6;
		SFC::BinaryExprs binaryExprs_58f7;
		SFC::Struct struct_58f8;
	};

	std::list< Match> _matches;
};

class Otherwise_5901
{
public:
	bool operator()( const Packets_t& localVars_5902, const Packets_t& structVars_5905, const Packets_t& binaryExprss_5908, const Packets_t& structs_590b, Packets_t& localVars_5904, Packets_t& structVars_5907, Packets_t& binaryExprss_590a, Packets_t& structs_590d);

protected:
	bool isInputUnique( const Udm::Object& localVar_5916, const Udm::Object& structVar_591f, const Udm::Object& binaryExprs_5928, const Udm::Object& struct_5931);
	void processInputPackets( const Packets_t& localVars_5902, const Packets_t& structVars_5905, const Packets_t& binaryExprss_5908, const Packets_t& structs_590b);
	bool patternMatcher( const Udm::Object& localVar_5914, const Udm::Object& structVar_591d, const Udm::Object& binaryExprs_5926, const Udm::Object& struct_592f);
	void outputAppender( const SFC::LocalVar& localVar_593b, const SFC::LocalVar& structVar_593d, const SFC::BinaryExprs& binaryExprs_593f, const SFC::Struct& struct_5941);

private:
	Packets_t* _localVar_590e;
	Packets_t* _structVar_590f;
	Packets_t* _binaryExprs_5910;
	Packets_t* _struct_5911;
	Packets_t _localVar_5912;
	Packets_t _structVar_591b;
	Packets_t _binaryExprs_5924;
	Packets_t _struct_592d;
	class Match
	{
	public:
		SFC::LocalVar localVar_5937;
		SFC::LocalVar structVar_5938;
		SFC::BinaryExprs binaryExprs_5939;
		SFC::Struct struct_593a;
	};

	std::list< Match> _matches;
};

class LastDot_5943
{
public:
	void operator()( const Packets_t& localVars_5944, const Packets_t& structVars_5946, const Packets_t& dotExprss_5948, const Packets_t& structs_594a);

protected:
	bool isInputUnique( const Udm::Object& localVar_5950, const Udm::Object& structVar_5959, const Udm::Object& dotExprs_5962, const Udm::Object& struct_596b);
	void processInputPackets( const Packets_t& localVars_5944, const Packets_t& structVars_5946, const Packets_t& dotExprss_5948, const Packets_t& structs_594a);
	bool patternMatcher( const Udm::Object& localVar_594e, const Udm::Object& structVar_5957, const Udm::Object& dotExprs_5960, const Udm::Object& struct_5969);
	void effector();

private:
	Packets_t _localVar_594c;
	Packets_t _structVar_5955;
	Packets_t _dotExprs_595e;
	Packets_t _struct_5967;
	class Match
	{
	public:
		SFC::LocalVar localVar_5971;
		SFC::LocalVar structVar_5972;
		SFC::BinaryExprs dotExprs_5973;
		SFC::Struct struct_5974;
	};

	std::list< Match> _matches;
};

class NextDot_5976
{
public:
	void operator()( const Packets_t& localVars_5977, const Packets_t& structVars_597a, const Packets_t& dotExprss_597c, const Packets_t& structs_597f, Packets_t& localVars_5979, Packets_t& nextDotExprss_597e, Packets_t& nextStructs_5981);

protected:
	bool isInputUnique( const Udm::Object& localVar_5989, const Udm::Object& structVar_5992, const Udm::Object& dotExprs_599b, const Udm::Object& struct_59a4);
	void processInputPackets( const Packets_t& localVars_5977, const Packets_t& structVars_597a, const Packets_t& dotExprss_597c, const Packets_t& structs_597f);
	bool patternMatcher( const Udm::Object& localVar_5987, const Udm::Object& structVar_5990, const Udm::Object& dotExprs_5999, const Udm::Object& struct_59a2);
	void effector();
	void outputAppender( const SFC::LocalVar& localVar_59b3, const SFC::BinaryExprs& nextDotExprs_59b5, const SFC::Struct& nextStruct_59b7);

private:
	Packets_t* _localVar_5982;
	Packets_t* _nextDotExprs_5983;
	Packets_t* _nextStruct_5984;
	Packets_t _localVar_5985;
	Packets_t _structVar_598e;
	Packets_t _dotExprs_5997;
	Packets_t _struct_59a0;
	class Match
	{
	public:
		SFC::LocalVar localVar_59ac;
		SFC::LocalVar structVar_59ad;
		SFC::BinaryExprs dotExprs_59ae;
		SFC::Struct struct_59af;
		SFC::Struct nextStruct_59b0;
	};

	std::list< Match> _matches;
};

class SortParamRefs_59b9
{
public:
	void operator()( const Packets_t& signalFlowModels_59ba, const Packets_t& functions_59bd, const Packets_t& contexts_59c0, Packets_t& parameterRefs_59bc, Packets_t& functions_59bf, Packets_t& contexts_59c2);

protected:
	bool isInputUnique( const Udm::Object& signalFlowModel_59ca, const Udm::Object& function_59d3, const Udm::Object& context_59dc);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& signalFlowModels_59ba, const Packets_t& functions_59bd, const Packets_t& contexts_59c0);
	bool patternMatcher( const Udm::Object& signalFlowModel_59c8, const Udm::Object& function_59d1, const Udm::Object& context_59da);
	void effector();
	void outputAppender( const CyberComposition::ParameterRef& parameterRef_59ec, const SFC::Function& function_59ee, const SFC::Arg& context_59f0);
	void sortOutputs();

private:
	Packets_t* _parameterRef_59c3;
	Packets_t* _function_59c4;
	Packets_t* _context_59c5;
	Packets_t _signalFlowModel_59c6;
	Packets_t _function_59cf;
	Packets_t _context_59d8;
	class Match
	{
	public:
		CyberComposition::SimulinkWrapper signalFlowModel_59e8;
		SFC::Function function_59e9;
		SFC::Arg context_59ea;
		CyberComposition::ParameterRef parameterRef_59eb;
	};

	std::list< Match> _matches;
};

class MakeTrigger_5a27
{
public:
	void operator()( const Packets_t& systems_5a28, const Packets_t& childSubsystems_5a2a, const Packets_t& systemFunctions_5a2c, Packets_t& systems_5a2e, Packets_t& childSubsystems_5a2f, Packets_t& compoundStatements_5a30);

protected:
	void callIsTriggered_5fa6( const Packets_t& systems_5a35, const Packets_t& childSubsystems_5a37, const Packets_t& systemFunctions_5a39);
	void callCreateTriggering_5faa( const Packets_t& systems_5acb, const Packets_t& childSubsystems_5acd, const Packets_t& systemFunctions_5acf);

private:
	Packets_t* _system_5a31;
	Packets_t* _childSubsystem_5a32;
	Packets_t* _compoundStatement_5a33;
};

class IsTriggered_5a34
{
public:
	void operator()( const Packets_t& systems_5a35, const Packets_t& childSubsystems_5a37, const Packets_t& systemFunctions_5a39, Packets_t& systems_5a3b, Packets_t& childSubsystems_5a3c, Packets_t& systemFunctions_5a3d, Packets_t& systems_5a3e, Packets_t& childSubsystems_5a3f, Packets_t& systemFunctions_5a40);

protected:
	void executeOne( const Packets_t& systems_5a35, const Packets_t& childSubsystems_5a37, const Packets_t& systemFunctions_5a39);
	bool isInputUnique( const Udm::Object& system_5a49, const Udm::Object& childSubsystem_5a50, const Udm::Object& systemFunction_5a57);

private:
	Packets_t* _system_5a41;
	Packets_t* _childSubsystem_5a42;
	Packets_t* _systemFunction_5a43;
	Packets_t* _system_5a44;
	Packets_t* _childSubsystem_5a45;
	Packets_t* _systemFunction_5a46;
	Packets_t _system_5a47;
	Packets_t _childSubsystem_5a4e;
	Packets_t _systemFunction_5a55;
};

class HasTriggers_5a5c
{
public:
	bool operator()( const Packets_t& subsystems_5a5d, const Packets_t& blocks_5a60, const Packets_t& mains_5a63, Packets_t& subsystems_5a5f, Packets_t& blocks_5a62, Packets_t& mains_5a65);

protected:
	bool isInputUnique( const Udm::Object& subsystem_5a6d, const Udm::Object& block_5a76, const Udm::Object& main_5a7f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_5a5d, const Packets_t& blocks_5a60, const Packets_t& mains_5a63);
	bool patternMatcher( const Udm::Object& subsystem_5a6b, const Udm::Object& block_5a74, const Udm::Object& main_5a7d);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_5a8f, const CyberComposition::Simulink::Subsystem& block_5a91, const SFC::Function& main_5a93);

private:
	Packets_t* _subsystem_5a66;
	Packets_t* _block_5a67;
	Packets_t* _main_5a68;
	Packets_t _subsystem_5a69;
	Packets_t _block_5a72;
	Packets_t _main_5a7b;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_5a8b;
		CyberComposition::Simulink::Subsystem block_5a8c;
		SFC::Function main_5a8d;
		CyberComposition::Simulink::TriggerPort triggerPort_5a8e;
	};

	std::list< Match> _matches;
};

class Otherwise_5a95
{
public:
	bool operator()( const Packets_t& subsystems_5a96, const Packets_t& blocks_5a99, const Packets_t& mains_5a9c, Packets_t& subsystems_5a98, Packets_t& blocks_5a9b, Packets_t& mains_5a9e);

protected:
	bool isInputUnique( const Udm::Object& subsystem_5aa6, const Udm::Object& block_5aaf, const Udm::Object& main_5ab8);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_5a96, const Packets_t& blocks_5a99, const Packets_t& mains_5a9c);
	bool patternMatcher( const Udm::Object& subsystem_5aa4, const Udm::Object& block_5aad, const Udm::Object& main_5ab6);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_5ac4, const CyberComposition::Simulink::Subsystem& block_5ac6, const SFC::Function& main_5ac8);

private:
	Packets_t* _subsystem_5a9f;
	Packets_t* _block_5aa0;
	Packets_t* _main_5aa1;
	Packets_t _subsystem_5aa2;
	Packets_t _block_5aab;
	Packets_t _main_5ab4;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_5ac1;
		CyberComposition::Simulink::Subsystem block_5ac2;
		SFC::Function main_5ac3;
	};

	std::list< Match> _matches;
};

class CreateTriggering_5aca
{
public:
	void operator()( const Packets_t& systems_5acb, const Packets_t& childSubsystems_5acd, const Packets_t& systemFunctions_5acf, Packets_t& systems_5ad1, Packets_t& childSubsystems_5ad2, Packets_t& conditionalBlocks_5ad3);

protected:
	void callCreateTriggerCondition_5f9b( const Packets_t& subsystems_5b05, const Packets_t& blocks_5b08, const Packets_t& mains_5b0c);
	void callAssignCondVal_5f9f( const Packets_t& triggerPorts_5ad8, const Packets_t& conditions_5adc);
	void callcreateTriggerOr_5fa2( const Packets_t& argdecls_5b48, const Packets_t& dts_5b4a, const Packets_t& conditions_5b4c);

private:
	Packets_t* _system_5ad4;
	Packets_t* _childSubsystem_5ad5;
	Packets_t* _conditionalBlock_5ad6;
};

class AssignCondVal_5ad7
{
public:
	void operator()( const Packets_t& triggerPorts_5ad8, const Packets_t& conditions_5adc, Packets_t& argDeclBases_5ada, Packets_t& dTs_5adb, Packets_t& conditions_5ade);

protected:
	bool isInputUnique( const Udm::Object& triggerPort_5ae6, const Udm::Object& condition_5aef);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& triggerPorts_5ad8, const Packets_t& conditions_5adc);
	bool patternMatcher( const Udm::Object& triggerPort_5ae4, const Udm::Object& condition_5aed);
	void effector();
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_5afe, const SFC::DT& dT_5b00, const SFC::UserCode& condition_5b02);

private:
	Packets_t* _argDeclBase_5adf;
	Packets_t* _dT_5ae0;
	Packets_t* _condition_5ae1;
	Packets_t _triggerPort_5ae2;
	Packets_t _condition_5aeb;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Port triggerPort_5afa;
		SFC::UserCode condition_5afb;
		SFC::ArgDeclBase argDeclBase_5afc;
		SFC::DT dT_5afd;
	};

	std::list< Match> _matches;
};

class CreateTriggerCondition_5b04
{
public:
	void operator()( const Packets_t& subsystems_5b05, const Packets_t& blocks_5b08, const Packets_t& mains_5b0c, Packets_t& subsystems_5b07, Packets_t& blocks_5b0a, Packets_t& triggerPorts_5b0b, Packets_t& conditionalBlocks_5b0e, Packets_t& conditions_5b0f);

protected:
	bool isInputUnique( const Udm::Object& subsystem_5b19, const Udm::Object& block_5b22, const Udm::Object& main_5b2b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_5b05, const Packets_t& blocks_5b08, const Packets_t& mains_5b0c);
	bool patternMatcher( const Udm::Object& subsystem_5b17, const Udm::Object& block_5b20, const Udm::Object& main_5b29);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_5b3d, const CyberComposition::Simulink::Block& block_5b3f, const CyberComposition::Simulink::TriggerPort& triggerPort_5b41, const SFC::ConditionalBlock& conditionalBlock_5b43, const SFC::UserCode& condition_5b45);

private:
	Packets_t* _subsystem_5b10;
	Packets_t* _block_5b11;
	Packets_t* _triggerPort_5b12;
	Packets_t* _conditionalBlock_5b13;
	Packets_t* _condition_5b14;
	Packets_t _subsystem_5b15;
	Packets_t _block_5b1e;
	Packets_t _main_5b27;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_5b37;
		CyberComposition::Simulink::Block block_5b38;
		SFC::Function main_5b39;
		CyberComposition::Simulink::TriggerPort triggerPort_5b3a;
	};

	std::list< Match> _matches;
};

class CreateTriggerOr_5b47
{
public:
	void operator()( const Packets_t& argdecls_5b48, const Packets_t& dts_5b4a, const Packets_t& conditions_5b4c);

protected:
	void callStructTest_5f8b( const Packets_t& argdecls_5edc, const Packets_t& dts_5ede, const Packets_t& conditions_5ee0);
	void callgetStructMembers_5f8f( const Packets_t& argDeclBases_5b4f, const Packets_t& structs_5b52, const Packets_t& userCodes_5b56);
	void callargDeclIsCondition_5f93( const Packets_t& argDeclBases_5f6e, const Packets_t& conditions_5f70);
	void callPlaceMember_5f96( const Packets_t& argdecls_5b8c, const Packets_t& structs_5b8e, const Packets_t& membs_5b90, const Packets_t& conditions_5b92);
};

class GetStructMembers_5b4e
{
public:
	void operator()( const Packets_t& argDeclBases_5b4f, const Packets_t& structs_5b52, const Packets_t& userCodes_5b56, Packets_t& argDeclBases_5b51, Packets_t& structs_5b54, Packets_t& localVars_5b55, Packets_t& userCodes_5b58);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_5b61, const Udm::Object& struct_5b6a, const Udm::Object& userCode_5b73);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_5b4f, const Packets_t& structs_5b52, const Packets_t& userCodes_5b56);
	bool patternMatcher( const Udm::Object& argDeclBase_5b5f, const Udm::Object& struct_5b68, const Udm::Object& userCode_5b71);
	void effector();
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_5b83, const SFC::Struct& struct_5b85, const SFC::LocalVar& localVar_5b87, const SFC::UserCode& userCode_5b89);
	void sortOutputs();

private:
	Packets_t* _argDeclBase_5b59;
	Packets_t* _struct_5b5a;
	Packets_t* _localVar_5b5b;
	Packets_t* _userCode_5b5c;
	Packets_t _argDeclBase_5b5d;
	Packets_t _struct_5b66;
	Packets_t _userCode_5b6f;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_5b7f;
		SFC::Struct struct_5b80;
		SFC::UserCode userCode_5b81;
		SFC::LocalVar localVar_5b82;
	};

	std::list< Match> _matches;
};

class PlaceMember_5b8b
{
public:
	void operator()( const Packets_t& argdecls_5b8c, const Packets_t& structs_5b8e, const Packets_t& membs_5b90, const Packets_t& conditions_5b92);

protected:
	void executeOne( const Packets_t& argdecls_5b8c, const Packets_t& structs_5b8e, const Packets_t& membs_5b90, const Packets_t& conditions_5b92);
	bool isInputUnique( const Udm::Object& argdecl_5b96, const Udm::Object& struct_5b9d, const Udm::Object& memb_5ba4, const Udm::Object& condition_5bab);
	void calltestMemberPos_5ebe( const Packets_t& argdecls_5d9f, const Packets_t& structs_5da1, const Packets_t& membs_5da3, const Packets_t& conditions_5da5);
	void callfirstBinaryExprs_5ec3( const Packets_t& argdecls_5bb1, const Packets_t& membs_5bb3, const Packets_t& conditions_5bb5);
	void callfirstBinaryExprs_5ec7( const Packets_t& argdecls_5bb1, const Packets_t& membs_5bb3, const Packets_t& conditions_5bb5);
	void callcreateOnlyExprs_5ecb( const Packets_t& argDeclBases_5d73, const Packets_t& localVars_5d75, const Packets_t& userCodes_5d77);
	void callCreateLastBinaryExprs_5ecf( const Packets_t& argDeclBases_5e93, const Packets_t& localVars_5e95, const Packets_t& binaryExprss_5e97);
	void callcreateFirstBinaryExprs_5ed3( const Packets_t& argDeclBases_5d46, const Packets_t& localVars_5d48, const Packets_t& userCodes_5d4a);
	void callCreateNextBinaryExprs_5ed7( const Packets_t& argDeclBases_5e66, const Packets_t& localVars_5e68, const Packets_t& binaryExprss_5e6a);

private:
	Packets_t _argdecl_5b94;
	Packets_t _struct_5b9b;
	Packets_t _memb_5ba2;
	Packets_t _condition_5ba9;
};

class FirstBinaryExprs_5bb0
{
public:
	void operator()( const Packets_t& argdecls_5bb1, const Packets_t& membs_5bb3, const Packets_t& conditions_5bb5, Packets_t& argdecls_5bb7, Packets_t& membs_5bb8, Packets_t& binaryexprss_5bb9, Packets_t& argdecls_5bba, Packets_t& membs_5bbb, Packets_t& conditions_5bbc);

protected:
	void callCondHasExprs_5d3d( const Packets_t& argdecls_5bc4, const Packets_t& membs_5bc6, const Packets_t& conditions_5bc8);
	void callgetLastBinaryExprs_5d41( const Packets_t& argdecls_5c5a, const Packets_t& membs_5c5c, const Packets_t& conditions_5c5e);

private:
	Packets_t* _argdecl_5bbd;
	Packets_t* _memb_5bbe;
	Packets_t* _binaryexprs_5bbf;
	Packets_t* _argdecl_5bc0;
	Packets_t* _memb_5bc1;
	Packets_t* _condition_5bc2;
};

class CondHasExprs_5bc3
{
public:
	void operator()( const Packets_t& argdecls_5bc4, const Packets_t& membs_5bc6, const Packets_t& conditions_5bc8, Packets_t& argdecls_5bca, Packets_t& membs_5bcb, Packets_t& conditions_5bcc, Packets_t& argdecls_5bcd, Packets_t& membs_5bce, Packets_t& conditions_5bcf);

protected:
	void executeOne( const Packets_t& argdecls_5bc4, const Packets_t& membs_5bc6, const Packets_t& conditions_5bc8);
	bool isInputUnique( const Udm::Object& argdecl_5bd8, const Udm::Object& memb_5bdf, const Udm::Object& condition_5be6);

private:
	Packets_t* _argdecl_5bd0;
	Packets_t* _memb_5bd1;
	Packets_t* _condition_5bd2;
	Packets_t* _argdecl_5bd3;
	Packets_t* _memb_5bd4;
	Packets_t* _condition_5bd5;
	Packets_t _argdecl_5bd6;
	Packets_t _memb_5bdd;
	Packets_t _condition_5be4;
};

class HasExpr_5beb
{
public:
	bool operator()( const Packets_t& argDeclBases_5bec, const Packets_t& localVars_5bef, const Packets_t& userCodes_5bf2, Packets_t& argDeclBases_5bee, Packets_t& localVars_5bf1, Packets_t& userCodes_5bf4);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_5bfc, const Udm::Object& localVar_5c05, const Udm::Object& userCode_5c0e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_5bec, const Packets_t& localVars_5bef, const Packets_t& userCodes_5bf2);
	bool patternMatcher( const Udm::Object& argDeclBase_5bfa, const Udm::Object& localVar_5c03, const Udm::Object& userCode_5c0c);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_5c1e, const SFC::LocalVar& localVar_5c20, const SFC::UserCode& userCode_5c22);

private:
	Packets_t* _argDeclBase_5bf5;
	Packets_t* _localVar_5bf6;
	Packets_t* _userCode_5bf7;
	Packets_t _argDeclBase_5bf8;
	Packets_t _localVar_5c01;
	Packets_t _userCode_5c0a;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_5c1a;
		SFC::LocalVar localVar_5c1b;
		SFC::UserCode userCode_5c1c;
		SFC::Exprs exprs_5c1d;
	};

	std::list< Match> _matches;
};

class Otherwise_5c24
{
public:
	bool operator()( const Packets_t& argDeclBases_5c25, const Packets_t& localVars_5c28, const Packets_t& userCodes_5c2b, Packets_t& argDeclBases_5c27, Packets_t& localVars_5c2a, Packets_t& userCodes_5c2d);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_5c35, const Udm::Object& localVar_5c3e, const Udm::Object& userCode_5c47);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_5c25, const Packets_t& localVars_5c28, const Packets_t& userCodes_5c2b);
	bool patternMatcher( const Udm::Object& argDeclBase_5c33, const Udm::Object& localVar_5c3c, const Udm::Object& userCode_5c45);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_5c53, const SFC::LocalVar& localVar_5c55, const SFC::UserCode& userCode_5c57);

private:
	Packets_t* _argDeclBase_5c2e;
	Packets_t* _localVar_5c2f;
	Packets_t* _userCode_5c30;
	Packets_t _argDeclBase_5c31;
	Packets_t _localVar_5c3a;
	Packets_t _userCode_5c43;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_5c50;
		SFC::LocalVar localVar_5c51;
		SFC::UserCode userCode_5c52;
	};

	std::list< Match> _matches;
};

class GetLastBinaryExprs_5c59
{
public:
	void operator()( const Packets_t& argdecls_5c5a, const Packets_t& membs_5c5c, const Packets_t& conditions_5c5e, Packets_t& argdecls_5c60, Packets_t& membs_5c61, Packets_t& binaryexprss_5c62);

protected:
	void callgetFirstBinaryExprs_5d35( const Packets_t& argDeclBases_5c67, const Packets_t& localVars_5c6a, const Packets_t& userCodes_5c6d);
	void callgetNextBinaryExprs_5d39( const Packets_t& argdecls_5ca0, const Packets_t& membs_5ca2, const Packets_t& binaryexprss_5ca4);

private:
	Packets_t* _argdecl_5c63;
	Packets_t* _memb_5c64;
	Packets_t* _binaryexprs_5c65;
};

class GetFirstBinaryExprs_5c66
{
public:
	void operator()( const Packets_t& argDeclBases_5c67, const Packets_t& localVars_5c6a, const Packets_t& userCodes_5c6d, Packets_t& argDeclBases_5c69, Packets_t& localVars_5c6c, Packets_t& binaryExprss_5c6f);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_5c77, const Udm::Object& localVar_5c80, const Udm::Object& userCode_5c89);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_5c67, const Packets_t& localVars_5c6a, const Packets_t& userCodes_5c6d);
	bool patternMatcher( const Udm::Object& argDeclBase_5c75, const Udm::Object& localVar_5c7e, const Udm::Object& userCode_5c87);
	void effector();
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_5c99, const SFC::LocalVar& localVar_5c9b, const SFC::BinaryExprs& binaryExprs_5c9d);

private:
	Packets_t* _argDeclBase_5c70;
	Packets_t* _localVar_5c71;
	Packets_t* _binaryExprs_5c72;
	Packets_t _argDeclBase_5c73;
	Packets_t _localVar_5c7c;
	Packets_t _userCode_5c85;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_5c95;
		SFC::LocalVar localVar_5c96;
		SFC::UserCode userCode_5c97;
		SFC::BinaryExprs binaryExprs_5c98;
	};

	std::list< Match> _matches;
};

class GetNextBinaryExprs_5c9f
{
public:
	void operator()( const Packets_t& argdecls_5ca0, const Packets_t& membs_5ca2, const Packets_t& binaryexprss_5ca4, Packets_t& argdecls_5ca6, Packets_t& membs_5ca7, Packets_t& binaryexprss_5ca8, Packets_t& argdecls_5ca9, Packets_t& membs_5caa, Packets_t& binaryexprss_5cab);

protected:
	void executeOne( const Packets_t& argdecls_5ca0, const Packets_t& membs_5ca2, const Packets_t& binaryexprss_5ca4);
	bool isInputUnique( const Udm::Object& argdecl_5cb4, const Udm::Object& memb_5cbb, const Udm::Object& binaryexprs_5cc2);

private:
	Packets_t* _argdecl_5cac;
	Packets_t* _memb_5cad;
	Packets_t* _binaryexprs_5cae;
	Packets_t* _argdecl_5caf;
	Packets_t* _memb_5cb0;
	Packets_t* _binaryexprs_5cb1;
	Packets_t _argdecl_5cb2;
	Packets_t _memb_5cb9;
	Packets_t _binaryexprs_5cc0;
};

class HasExpr_5cc7
{
public:
	bool operator()( const Packets_t& argDeclBases_5cc8, const Packets_t& localVars_5ccb, const Packets_t& binaryExprss_5cce, Packets_t& argDeclBases_5cca, Packets_t& localVars_5ccd, Packets_t& rightBinaryExprss_5cd0);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_5cd8, const Udm::Object& localVar_5ce1, const Udm::Object& binaryExprs_5cea);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_5cc8, const Packets_t& localVars_5ccb, const Packets_t& binaryExprss_5cce);
	bool patternMatcher( const Udm::Object& argDeclBase_5cd6, const Udm::Object& localVar_5cdf, const Udm::Object& binaryExprs_5ce8);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_5cfa, const SFC::LocalVar& localVar_5cfc, const SFC::BinaryExprs& rightBinaryExprs_5cfe);

private:
	Packets_t* _argDeclBase_5cd1;
	Packets_t* _localVar_5cd2;
	Packets_t* _rightBinaryExprs_5cd3;
	Packets_t _argDeclBase_5cd4;
	Packets_t _localVar_5cdd;
	Packets_t _binaryExprs_5ce6;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_5cf6;
		SFC::LocalVar localVar_5cf7;
		SFC::BinaryExprs binaryExprs_5cf8;
		SFC::BinaryExprs rightBinaryExprs_5cf9;
	};

	std::list< Match> _matches;
};

class Otherwise_5d00
{
public:
	bool operator()( const Packets_t& argDeclBases_5d01, const Packets_t& localVars_5d04, const Packets_t& binaryExprss_5d07, Packets_t& argDeclBases_5d03, Packets_t& localVars_5d06, Packets_t& binaryExprss_5d09);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_5d11, const Udm::Object& localVar_5d1a, const Udm::Object& binaryExprs_5d23);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_5d01, const Packets_t& localVars_5d04, const Packets_t& binaryExprss_5d07);
	bool patternMatcher( const Udm::Object& argDeclBase_5d0f, const Udm::Object& localVar_5d18, const Udm::Object& binaryExprs_5d21);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_5d2f, const SFC::LocalVar& localVar_5d31, const SFC::BinaryExprs& binaryExprs_5d33);

private:
	Packets_t* _argDeclBase_5d0a;
	Packets_t* _localVar_5d0b;
	Packets_t* _binaryExprs_5d0c;
	Packets_t _argDeclBase_5d0d;
	Packets_t _localVar_5d16;
	Packets_t _binaryExprs_5d1f;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_5d2c;
		SFC::LocalVar localVar_5d2d;
		SFC::BinaryExprs binaryExprs_5d2e;
	};

	std::list< Match> _matches;
};

class CreateFirstBinaryExprs_5d45
{
public:
	void operator()( const Packets_t& argDeclBases_5d46, const Packets_t& localVars_5d48, const Packets_t& userCodes_5d4a);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_5d50, const Udm::Object& localVar_5d59, const Udm::Object& userCode_5d62);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_5d46, const Packets_t& localVars_5d48, const Packets_t& userCodes_5d4a);
	bool patternMatcher( const Udm::Object& argDeclBase_5d4e, const Udm::Object& localVar_5d57, const Udm::Object& userCode_5d60);
	void effector();

private:
	Packets_t _argDeclBase_5d4c;
	Packets_t _localVar_5d55;
	Packets_t _userCode_5d5e;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_5d6b;
		SFC::LocalVar localVar_5d6c;
		SFC::UserCode userCode_5d6d;
	};

	std::list< Match> _matches;
};

class CreateOnlyExprs_5d72
{
public:
	void operator()( const Packets_t& argDeclBases_5d73, const Packets_t& localVars_5d75, const Packets_t& userCodes_5d77);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_5d7d, const Udm::Object& localVar_5d86, const Udm::Object& userCode_5d8f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_5d73, const Packets_t& localVars_5d75, const Packets_t& userCodes_5d77);
	bool patternMatcher( const Udm::Object& argDeclBase_5d7b, const Udm::Object& localVar_5d84, const Udm::Object& userCode_5d8d);
	void effector();

private:
	Packets_t _argDeclBase_5d79;
	Packets_t _localVar_5d82;
	Packets_t _userCode_5d8b;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_5d98;
		SFC::LocalVar localVar_5d99;
		SFC::UserCode userCode_5d9a;
	};

	std::list< Match> _matches;
};

class TestMemberPos_5d9e
{
public:
	void operator()( const Packets_t& argdecls_5d9f, const Packets_t& structs_5da1, const Packets_t& membs_5da3, const Packets_t& conditions_5da5, Packets_t& argdecls_5da7, Packets_t& structs_5da8, Packets_t& membs_5da9, Packets_t& conditions_5daa, Packets_t& argdecls_5dab, Packets_t& structs_5dac, Packets_t& membs_5dad, Packets_t& conditions_5dae);

protected:
	void executeOne( const Packets_t& argdecls_5d9f, const Packets_t& structs_5da1, const Packets_t& membs_5da3, const Packets_t& conditions_5da5);
	bool isInputUnique( const Udm::Object& argdecl_5db9, const Udm::Object& struct_5dc0, const Udm::Object& memb_5dc7, const Udm::Object& condition_5dce);

private:
	Packets_t* _argdecl_5daf;
	Packets_t* _struct_5db0;
	Packets_t* _memb_5db1;
	Packets_t* _condition_5db2;
	Packets_t* _argdecl_5db3;
	Packets_t* _struct_5db4;
	Packets_t* _memb_5db5;
	Packets_t* _condition_5db6;
	Packets_t _argdecl_5db7;
	Packets_t _struct_5dbe;
	Packets_t _memb_5dc5;
	Packets_t _condition_5dcc;
};

class NotLastMember_5dd3
{
public:
	bool operator()( const Packets_t& argDeclBases_5dd4, const Packets_t& structs_5dd7, const Packets_t& localVars_5dda, const Packets_t& userCodes_5ddd, Packets_t& argDeclBases_5dd6, Packets_t& structs_5dd9, Packets_t& localVars_5ddc, Packets_t& userCodes_5ddf);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_5de8, const Udm::Object& struct_5df1, const Udm::Object& localVar_5dfa, const Udm::Object& userCode_5e03);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::LocalVar& LocalVar, SFC::Struct& Struct, SFC::UserCode& UserCode);
	void processInputPackets( const Packets_t& argDeclBases_5dd4, const Packets_t& structs_5dd7, const Packets_t& localVars_5dda, const Packets_t& userCodes_5ddd);
	bool patternMatcher( const Udm::Object& argDeclBase_5de6, const Udm::Object& struct_5def, const Udm::Object& localVar_5df8, const Udm::Object& userCode_5e01);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_5e14, const SFC::Struct& struct_5e16, const SFC::LocalVar& localVar_5e18, const SFC::UserCode& userCode_5e1a);

private:
	Packets_t* _argDeclBase_5de0;
	Packets_t* _struct_5de1;
	Packets_t* _localVar_5de2;
	Packets_t* _userCode_5de3;
	Packets_t _argDeclBase_5de4;
	Packets_t _struct_5ded;
	Packets_t _localVar_5df6;
	Packets_t _userCode_5dff;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_5e0c;
		SFC::Struct struct_5e0d;
		SFC::LocalVar localVar_5e0e;
		SFC::UserCode userCode_5e0f;
	};

	std::list< Match> _matches;
};

class Otherwise_5e1c
{
public:
	bool operator()( const Packets_t& argDeclBases_5e1d, const Packets_t& structs_5e20, const Packets_t& localVars_5e23, const Packets_t& userCodes_5e26, Packets_t& argDeclBases_5e1f, Packets_t& structs_5e22, Packets_t& localVars_5e25, Packets_t& userCodes_5e28);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_5e31, const Udm::Object& struct_5e3a, const Udm::Object& localVar_5e43, const Udm::Object& userCode_5e4c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::ArgDeclBase& ArgDeclBase, SFC::LocalVar& LocalVar, SFC::Struct& Struct, SFC::UserCode& UserCode);
	void processInputPackets( const Packets_t& argDeclBases_5e1d, const Packets_t& structs_5e20, const Packets_t& localVars_5e23, const Packets_t& userCodes_5e26);
	bool patternMatcher( const Udm::Object& argDeclBase_5e2f, const Udm::Object& struct_5e38, const Udm::Object& localVar_5e41, const Udm::Object& userCode_5e4a);
	void outputAppender( const SFC::ArgDeclBase& argDeclBase_5e5d, const SFC::Struct& struct_5e5f, const SFC::LocalVar& localVar_5e61, const SFC::UserCode& userCode_5e63);

private:
	Packets_t* _argDeclBase_5e29;
	Packets_t* _struct_5e2a;
	Packets_t* _localVar_5e2b;
	Packets_t* _userCode_5e2c;
	Packets_t _argDeclBase_5e2d;
	Packets_t _struct_5e36;
	Packets_t _localVar_5e3f;
	Packets_t _userCode_5e48;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_5e55;
		SFC::Struct struct_5e56;
		SFC::LocalVar localVar_5e57;
		SFC::UserCode userCode_5e58;
	};

	std::list< Match> _matches;
};

class CreateNextBinaryExprs_5e65
{
public:
	void operator()( const Packets_t& argDeclBases_5e66, const Packets_t& localVars_5e68, const Packets_t& binaryExprss_5e6a);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_5e70, const Udm::Object& localVar_5e79, const Udm::Object& binaryExprs_5e82);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_5e66, const Packets_t& localVars_5e68, const Packets_t& binaryExprss_5e6a);
	bool patternMatcher( const Udm::Object& argDeclBase_5e6e, const Udm::Object& localVar_5e77, const Udm::Object& binaryExprs_5e80);
	void effector();

private:
	Packets_t _argDeclBase_5e6c;
	Packets_t _localVar_5e75;
	Packets_t _binaryExprs_5e7e;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_5e8b;
		SFC::LocalVar localVar_5e8c;
		SFC::BinaryExprs binaryExprs_5e8d;
	};

	std::list< Match> _matches;
};

class CreateLastBinaryExprs_5e92
{
public:
	void operator()( const Packets_t& argDeclBases_5e93, const Packets_t& localVars_5e95, const Packets_t& binaryExprss_5e97);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_5e9d, const Udm::Object& localVar_5ea6, const Udm::Object& binaryExprs_5eaf);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_5e93, const Packets_t& localVars_5e95, const Packets_t& binaryExprss_5e97);
	bool patternMatcher( const Udm::Object& argDeclBase_5e9b, const Udm::Object& localVar_5ea4, const Udm::Object& binaryExprs_5ead);
	void effector();

private:
	Packets_t _argDeclBase_5e99;
	Packets_t _localVar_5ea2;
	Packets_t _binaryExprs_5eab;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_5eb8;
		SFC::LocalVar localVar_5eb9;
		SFC::BinaryExprs binaryExprs_5eba;
	};

	std::list< Match> _matches;
};

class StructTest_5edb
{
public:
	void operator()( const Packets_t& argdecls_5edc, const Packets_t& dts_5ede, const Packets_t& conditions_5ee0, Packets_t& argdecls_5ee2, Packets_t& structs_5ee3, Packets_t& conditions_5ee4, Packets_t& argdecls_5ee5, Packets_t& dts_5ee6, Packets_t& conditions_5ee7);

protected:
	void executeOne( const Packets_t& argdecls_5edc, const Packets_t& dts_5ede, const Packets_t& conditions_5ee0);
	bool isInputUnique( const Udm::Object& argdecl_5ef0, const Udm::Object& dt_5ef7, const Udm::Object& condition_5efe);

private:
	Packets_t* _argdecl_5ee8;
	Packets_t* _struct_5ee9;
	Packets_t* _condition_5eea;
	Packets_t* _argdecl_5eeb;
	Packets_t* _dt_5eec;
	Packets_t* _condition_5eed;
	Packets_t _argdecl_5eee;
	Packets_t _dt_5ef5;
	Packets_t _condition_5efc;
};

class IsStruct_5f03
{
public:
	bool operator()( const Packets_t& argVarBases_5f04, const Packets_t& structs_5f07, const Packets_t& conditions_5f0a, Packets_t& argVarBases_5f06, Packets_t& structs_5f09, Packets_t& conditions_5f0c);

protected:
	bool isInputUnique( const Udm::Object& argVarBase_5f14, const Udm::Object& struct_5f1d, const Udm::Object& condition_5f26);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argVarBases_5f04, const Packets_t& structs_5f07, const Packets_t& conditions_5f0a);
	bool patternMatcher( const Udm::Object& argVarBase_5f12, const Udm::Object& struct_5f1b, const Udm::Object& condition_5f24);
	void outputAppender( const SFC::ArgDeclBase& argVarBase_5f32, const SFC::Struct& struct_5f34, const SFC::UserCode& condition_5f36);

private:
	Packets_t* _argVarBase_5f0d;
	Packets_t* _struct_5f0e;
	Packets_t* _condition_5f0f;
	Packets_t _argVarBase_5f10;
	Packets_t _struct_5f19;
	Packets_t _condition_5f22;
	class Match
	{
	public:
		SFC::ArgDeclBase argVarBase_5f2f;
		SFC::Struct struct_5f30;
		SFC::UserCode condition_5f31;
	};

	std::list< Match> _matches;
};

class Otherwise_5f38
{
public:
	bool operator()( const Packets_t& argVarBases_5f39, const Packets_t& dTs_5f3c, const Packets_t& conditions_5f3f, Packets_t& argVarBases_5f3b, Packets_t& dTs_5f3e, Packets_t& conditions_5f41);

protected:
	bool isInputUnique( const Udm::Object& argVarBase_5f49, const Udm::Object& dT_5f52, const Udm::Object& condition_5f5b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argVarBases_5f39, const Packets_t& dTs_5f3c, const Packets_t& conditions_5f3f);
	bool patternMatcher( const Udm::Object& argVarBase_5f47, const Udm::Object& dT_5f50, const Udm::Object& condition_5f59);
	void outputAppender( const SFC::ArgDeclBase& argVarBase_5f67, const SFC::DT& dT_5f69, const SFC::UserCode& condition_5f6b);

private:
	Packets_t* _argVarBase_5f42;
	Packets_t* _dT_5f43;
	Packets_t* _condition_5f44;
	Packets_t _argVarBase_5f45;
	Packets_t _dT_5f4e;
	Packets_t _condition_5f57;
	class Match
	{
	public:
		SFC::ArgDeclBase argVarBase_5f64;
		SFC::DT dT_5f65;
		SFC::UserCode condition_5f66;
	};

	std::list< Match> _matches;
};

class ArgDeclIsCondition_5f6d
{
public:
	void operator()( const Packets_t& argDeclBases_5f6e, const Packets_t& conditions_5f70);

protected:
	bool isInputUnique( const Udm::Object& argDeclBase_5f76, const Udm::Object& condition_5f7f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& argDeclBases_5f6e, const Packets_t& conditions_5f70);
	bool patternMatcher( const Udm::Object& argDeclBase_5f74, const Udm::Object& condition_5f7d);
	void effector();

private:
	Packets_t _argDeclBase_5f72;
	Packets_t _condition_5f7b;
	class Match
	{
	public:
		SFC::ArgDeclBase argDeclBase_5f88;
		SFC::UserCode condition_5f89;
	};

	std::list< Match> _matches;
};

