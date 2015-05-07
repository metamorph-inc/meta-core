#ifndef __MOCODEGEN_H__
#define __MOCODEGEN_H__

#include "CyberComposition.h"

class MoCodeGen
{
public:
//	MoCodeGen(CyPhyML::Component &cyphyComponent, const std::string &outputDirectory, const std::string logFile);
//	MoCodeGen(CyPhyML::TestBench &testbench, const std::string &outputDirectory);
	MoCodeGen(CyberComposition::SimulinkWrapper &slmodel, const std::string &outputDirectory, const std::string logFile);
	void gen();
	void setPackageName(const std::string &pkg);
private:
	void genWrapper_source();
	void genWrapper_proj();
	void genModelica();
	void genPackageMo();
	std::string generateGUID();
	std::string convertMoType(const std::string &slType);
	//std::string getDataVarFullPath(const CyPhyML::SF_DataParameterBase &obj);
	//std::string getParaVarFullPath(const CyberComposition::Simulink::SF_Parameter &param);
	std::string getParaVarFullPath(const CyberComposition::ParameterBase &param);
//	CyPhyML::BusPort getBusPort(const CyPhyML::SignalFlowBusPortInterface &busportInterface);
	int getValueArraySize(const CyberComposition::Simulink::SF_Parameter &sf_parameter);

	//CyPhyML::Component _com;
	CyberComposition::SimulinkWrapper _sl;
	std::string _slName;
	std::string _directory;
	std::string _wrapperName;
	std::string _topSubSystemName;
	std::string _contextName;
	std::string _pkgName;
	std::string _logFileName;
	
	list<CyberComposition::Simulink::Subsystem> _topSubSystems;

	list<CyberComposition::Simulink::InputPort> _inputs;
	list<CyberComposition::Simulink::OutputPort> _outputs;
	map<CyberComposition::Simulink::InputPort, CyberComposition::Simulink::TypeBaseRef> _inputMap;
	map<CyberComposition::Simulink::OutputPort, CyberComposition::Simulink::TypeBaseRef>  _outputMap;
//	map<CyPhyML::SignalFlowBusPortInterface, CyPhyML::BusPort> _busportMap;
	map<CyberComposition::ParameterRef, std::string > _parameterRefMap;
	map<CyberComposition::ParameterRef, std::string > _sfdataRefMap;

	set<std::string> _hfiles;
	set<std::string> _cfiles;
	std::string _mainArgs;

	struct InputPortSorter {
		bool operator()( const CyberComposition::Simulink::InputPort &port1, const CyberComposition::Simulink::InputPort &port2 ) const {
				return static_cast< int >(port1.Number() ) < static_cast< int >( port2.Number() );
			}
		};

	struct OutputPortSorter {
		bool operator()( const CyberComposition::Simulink::OutputPort &port1, const CyberComposition::Simulink::OutputPort &port2 ) const {
				return static_cast< int >(port1.Number() ) < static_cast< int >( port2.Number() );
			}
		};
};

#endif