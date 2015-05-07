#ifndef __FMUCODEGEN_H__
#define __FMUCODEGEN_H__

#include "SFC.h"
#include "FMI.h"

class FMUCodeGen
{
public:
	FMUCodeGen(SFC::Project &prj, const std::string &topSubsystemName, const std::string &outputDirectory);
	void gen();

private:
	SFC::Project _sfprj;
	std::string _topSubsystemName;
	std::string _fmiFile;
	std::string _cWrapperFile;

	std::string _define;
	std::string _setStartValues;
	std::string _getReal;

	int _nReal;
	int _nInt;
	int _nBool;
	int _nStr;
	int _nState;
	int _nEvent;
	
	void getArgs();
	std::string generateGUID();
	void genModelVariables(FMI::ModelVariables &mvars);
	void processArg(FMI::ModelVariables &mvars, const SFC::Arg &arg);
};

#endif