#include "SfuncCodeTemplate.hpp"

SfuncCodeTemplate::TemplateString::TemplateString( void ) {

	std::ostringstream templateStringStream;

	templateStringStream <<
"#define S_FUNCTION_NAME  %s"                                  << std::endl <<
"#define S_FUNCTION_LEVEL 2"                                   << std::endl <<
                                                                  std::endl <<
"#include \"simstruc.h\""                                      << std::endl <<
"#include \"%s\""                                              << std::endl <<
                                                                  std::endl <<
"static void mdlInitializeSizes(SimStruct *S)"                 << std::endl <<
"{"                                                            << std::endl <<
"    ssSetNumSFcnParams(S, 0);"                                << std::endl <<
"    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {"  << std::endl <<
"        return;"                                              << std::endl <<
"    }"                                                        << std::endl <<
"    if (!ssSetNumInputPorts(S, 1)) return;"                   << std::endl <<
"    ssSetInputPortWidth(S, 0, %d);"                           << std::endl <<
"    ssSetInputPortDirectFeedThrough(S, 0, 1);"                << std::endl <<
"    if (!ssSetNumOutputPorts(S, 1)) return;"                  << std::endl <<
"    ssSetOutputPortWidth(S, 0, %d);"                          << std::endl <<
"    ssSetNumSampleTimes(S, 1);"                               << std::endl <<
"    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE |"          << std::endl <<
"                 SS_OPTION_USE_TLC_WITH_ACCELERATOR);"        << std::endl <<
"    %s"                                                       << std::endl <<
"}"                                                            << std::endl <<
                                                                  std::endl <<
"static void mdlInitializeSampleTimes(SimStruct *S)"           << std::endl <<
"{"                                                            << std::endl <<
"    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);"            << std::endl <<
"    ssSetOffsetTime(S, 0, 0.0);"                              << std::endl <<
"}"                                                            << std::endl <<
                                                                  std::endl <<
"static void mdlOutputs(SimStruct *S, int_T tid)"              << std::endl <<
"{"                                                            << std::endl <<
"    %s"                                                       << std::endl <<
"}"                                                            << std::endl <<
                                                                  std::endl <<
"static void mdlTerminate(SimStruct *S)"                       << std::endl <<
"{"                                                            << std::endl <<
"}"                                                            << std::endl <<
                                                                  std::endl <<
                                                                  std::endl <<
                                                                  std::endl <<
"#ifdef  MATLAB_MEX_FILE"                                      << std::endl <<
"#include \"simulink.c\""                                      << std::endl <<
"#else"                                                        << std::endl <<
"#include \"cg_sfun.h\""                                       << std::endl <<
"#endif"                                                       << std::endl <<
                                                                  std::endl;

	_templateString = templateStringStream.str();
}
