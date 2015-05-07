#ifndef RUNSF_CODEGEN
#define RUNSF_CODEGEN

#include "CyberComposition.h"

void __declspec(dllexport) runCyberSF_CodeGen_dllimported(const CyberComposition::SimulinkWrapper &slmodel, std::string cyber_file, Uml::Diagram &cyber_meta_diagram, std::string logFile, std::string directory="");
void runCyberSF_CodeGen(const CyberComposition::Simulink::Subsystem &sys, std::string cyber_file, Udm::DataNetwork* sdn_cyber, std::string directory="");
void runCyberSF_CodeGen(const CyberComposition::SimulinkWrapper &slmodel, std::string cyber_file, Udm::DataNetwork* sdn_cyber, std::string directory="");

#endif