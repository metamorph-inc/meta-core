#include "FMUCodeGen.h"
#include "FMI.h"
#include <fstream>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "Rpc.h"

#include <UdmDom.h>

FMUCodeGen::FMUCodeGen(SFC::Project &prj, const std::string &topSubsystemName, const std::string &outputDirectory)
	:_topSubsystemName(topSubsystemName), _sfprj(prj),_nReal(0),_nInt(0),_nBool(0),_nStr(0),_nState(0),_nEvent(0)
{
	_fmiFile = outputDirectory+"\\"+topSubsystemName+"_Description.xml";
	_cWrapperFile = outputDirectory+"\\"+topSubsystemName+"_fmu.c";
}

void FMUCodeGen::gen()
{
	std::string guid = generateGUID();

	UdmDom::DomDataNetwork fmiDN(FMI::diagram);
	fmiDN.CreateNew(_fmiFile, "FMI.xsd", FMI::fmiModelDescription::meta);

	FMI::fmiModelDescription fmiMD = FMI::fmiModelDescription::Cast(fmiDN.GetRootObject());
	fmiMD.fmiVersion() = "1.0";
	fmiMD.modelName() = _topSubsystemName;
	fmiMD.modelIdentifier() = _topSubsystemName;
	fmiMD.guid() = guid;
	fmiMD.numberOfContinuousStates() = 0;
	fmiMD.numberOfEventIndicators() = 0;

	FMI::ModelVariables mvars = FMI::ModelVariables::Create(fmiMD);
	genModelVariables(mvars);
	
	fmiDN.CloseWithUpdate();
	
	std::ofstream fmu_file;
	fmu_file.open(_cWrapperFile.c_str());

	fmu_file << "// define class name and unique id\n";
	fmu_file << "#define MODEL_IDENTIFIER " << _topSubsystemName << "\n";
	fmu_file << "#define MODEL_GUID \"" << guid << "\"\n";
	fmu_file <<	"\n";
	fmu_file << "// define model size\n";
	fmu_file << "#define NUMBER_OF_REALS "+boost::lexical_cast<string>(_nReal)+"\n";
	fmu_file << "#define NUMBER_OF_INTEGERS "+boost::lexical_cast<string>(_nInt)+"\n";
	fmu_file << "#define NUMBER_OF_BOOLEANS 0\n";
	fmu_file << "#define NUMBER_OF_STRINGS "+boost::lexical_cast<string>(_nStr)+"\n";
	fmu_file << "#define NUMBER_OF_STATES 0\n";
	fmu_file << "#define NUMBER_OF_EVENT_INDICATORS 0\n";
	fmu_file <<	"\n";
	fmu_file << "// include fmu header files, typedefs and macros\n";
	fmu_file << "#include \"fmuTemplate.h\"\n";
	fmu_file << "#include \"" << _topSubsystemName <<"_sl.h\"\n";
	fmu_file <<	"\n";
	fmu_file << _define;
	fmu_file <<	"\n";
	fmu_file << "static " << _topSubsystemName << "_context context;\n";
	fmu_file << "\n";
	fmu_file << "// called by fmiInstantiateModel\n";
	fmu_file << "// Set values for all variables that define a start value\n";
	fmu_file << "// Settings used unless changed by fmiSetX before fmiInitialize\n";
	fmu_file << "void setStartValues(ModelInstance *comp) {\n";
	fmu_file << _setStartValues <<"\n";
	fmu_file << "}\n";
	fmu_file << "\n";
	fmu_file << "// called by fmiInitialize() after setting eventInfo to defaults\n";
	fmu_file << "// Used to set the first time event, if any.\n";
	fmu_file << "void initialize(ModelInstance* comp, fmiEventInfo* eventInfo) {\n";
    fmu_file << "\teventInfo->upcomingTimeEvent   = fmiFalse;\n";
    fmu_file << "\teventInfo->nextEventTime       = 0;\n";
	fmu_file << "}\n";
	fmu_file << "// called by fmiGetReal, fmiGetContinuousStates and fmiGetDerivatives\n";
	fmu_file << "fmiReal getReal(ModelInstance* comp, fmiValueReference vr){\n";
    fmu_file << "\tswitch (vr) {\n";
	fmu_file << _getReal;
	fmu_file << "\t\tdefault: return 0.0;\n";
	fmu_file << "\t}\n";
	fmu_file << "}\n";
	fmu_file << "\n";
	fmu_file << "// called by fmiEventUpdate() after setting eventInfo to defaults\n";
	fmu_file << "void eventUpdate(ModelInstance* comp, fmiEventInfo* eventInfo) {\n";
	fmu_file << "} \n";
	fmu_file << "\n";
	fmu_file << "// include code that implements the FMI based on the above definitions\n";
	fmu_file << "#include \"fmuTemplate.c\"\n";

	fmu_file.close();
}

std::string FMUCodeGen::generateGUID()
{
	UUID uuid;     
	UuidCreate ( &uuid );      
	unsigned char * str;     
	UuidToStringA ( &uuid, &str );      
	std::string guid( ( char* ) str );
	guid = "{"+guid+"}";
	RpcStringFreeA ( &str ); 
	return guid;
}

class ArgIndexPredicate {
public:
	bool operator() (const SFC::Arg &arg1, const SFC::Arg &arg2) {
		return arg1.argIndex() < arg2.argIndex();
	}
};

void FMUCodeGen::genModelVariables(FMI::ModelVariables &mvars)
{
	set<SFC::Program> programs = _sfprj.Program_kind_children();
	for(set<SFC::Program>::iterator it=programs.begin();it!=programs.end();++it)
	{
		set<SFC::Class> clss = (*it).Class_kind_children();
		for(set<SFC::Class>::iterator clss_it=clss.begin();clss_it!=clss.end();++clss_it)
		{
			set<SFC::Function> funcs = (*clss_it).Function_kind_children();
			for(set<SFC::Function>::iterator funcs_it=funcs.begin();funcs_it!=funcs.end();++funcs_it)
			{
				SFC::Function func = *funcs_it;
				set<SFC::FunctionCall> func_calls = func.caller();
				if(func_calls.empty())
				{
					set<SFC::Arg, ArgIndexPredicate> args = func.Arg_kind_children_sorted(ArgIndexPredicate());
					for(set<SFC::Arg, ArgIndexPredicate>::iterator args_it=args.begin();args_it!=args.end();++args_it)
					{
						const SFC::Arg &arg = *args_it;
						if((std::string)arg.type()!="")
							processArg(mvars, arg);
					}
				}
			}
		}
	}
}

void FMUCodeGen::processArg(FMI::ModelVariables &mvars, const SFC::Arg &arg)
{
	std::string argname = arg.name();
	std::string argtype = arg.type();
	long long rv = arg.argIndex();
	bool isOutput = arg.ptr();

	//gen FMI scalarvariable
	FMI::ScalarVariable sv = FMI::ScalarVariable::Create(mvars);
	sv.name() = argname;
	sv.valueReference() = rv;
	sv.description() = argname;
	sv.causality() = isOutput ? "output" : "input";
	if(argtype == "double")
	{
		_nReal++;
		FMI::Real freal = FMI::Real::Create(sv);
	}
	else if(argtype == "int")
	{
		_nInt++;
		FMI::Integer fint = FMI::Integer::Create(sv);
	}
	else //string
	{
		_nStr++;
		FMI::String fstr = FMI::String::Create(sv);
	}

	//FMU c 
	std::string d = "#define "+argname+"_ \t"+boost::lexical_cast<string>(rv)+"\n";
	_define += d;
	std::string ssv = "\tr("+argname+"_) = 0.0;\n";
	_setStartValues += ssv;
	std::string gr = "\t\tcase "+argname+"_\t: return r("+argname+"_);\n";// case TCTopOutRPM_   : return r(TCTopOutRPM_);
	_getReal += gr;
}