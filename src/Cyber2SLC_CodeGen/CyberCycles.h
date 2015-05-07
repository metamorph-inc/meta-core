#ifndef _CYBER_CYCLES_H_
#define _CYBER_CYCLES_H_

#include "CycleGraphs.h"
#include "JohnsonsAlgorithm.h"
#include "CyberComposition.h"

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <exception>

class CyberCycles
{
public:

	// Create the graph
	CyberCycles( const CyberComposition::Simulink::Subsystem & sub );
	virtual ~CyberCycles() { _log.close(); }

	bool FindCycles();

	void CollectCycleList( std::vector< std::vector < std::string > > & cycleList ) { cycleList = _cycleList; }

private:

	// Cycle graph
	CompGraph * _cg;

	// Log file
	std::ofstream _log;

	// Path string
	std::vector< std::string > _curPath;

	// UDM UniqueId -> vertex integer
	std::map< unsigned long long, idx_t > _idToVertexMap;

	// vertex integer -> path string
	std::map< idx_t, std::string > _vertexToPathStrMap;

	// Cycle collection
	std::vector< std::vector< std::string > > _cycleList;

	// Recursively build cycle graph
	void BuildCycleGraph( const CyberComposition::Simulink::Subsystem & sub );
	std::string MakePathName( std::string & curObjName );

};

#endif // _FIND_CYCLES_H_