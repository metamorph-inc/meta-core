#ifndef RUNSL_CODEGEN
#define RUNSL_CODEGEN

#include "CyberComposition.h"

void __declspec(dllimport) runCyberSF_CodeGen_dllimported(const CyberComposition::SimulinkWrapper &slmodel, std::string cyber_file, Uml::Diagram &cyber_meta_diagram, std::string logFile, std::string directory="");
bool __declspec(dllexport) runCyberSL_CodeGen_dllimported(const CyberComposition::SimulinkWrapper &slmodel, std::string cyber_file, Uml::Diagram &cyber_meta_diagram, std::string directory="");
bool runCyberSL_CodeGen(const CyberComposition::SimulinkWrapper &slmodel, std::string cyber_file, Udm::DataNetwork* sdn_cyber, std::string directory="");
void logGeneratedFiles(std::string workDir);

class ComponentCodeGen
{
public:
	ComponentCodeGen(CyberComposition::SimulinkWrapper &slmodel, const std::string &outputDirectory, const std::string &comname);
	void runCodeGen(const std::string &logFile);
	set<std::string> getGenerated_H_Files();
	set<std::string> getGenerated_C_Files();
private:
	CyberComposition::SimulinkWrapper _slmodel;
	std::string _slname;
	std::string _directory;
	set<std::string> _cfiles;
	set<std::string> _hfiles;
	bool errorReported;

public:
	bool getError( void ) {
		return errorReported;
	}
	void setError( void ) {
		errorReported = true;
	}
};

#endif