#ifndef FORMULA_TRAVERSE_H
#define FORMULA_TRAVERSE_H

#include "Uml.h"
#include "UmlExt.h"
#include "CyPhyML.h"

#include "UnitUtil.h"

/** \file
    \brief Definition of FormulaTraverse class used to traverse a model and calls functions in EvaluateFormula.h to evaluate formulas.
*/

using namespace std;


void Traverse(const Udm::Object &focusObject);
void Traverse(const std::set<Udm::Object> &selectedObjects);

void Cleanup(multimap<std::string, double*> &parameters);


  /** \brief New traversal class that is used currently.
  */
class NewTraverser 
{
public:
	NewTraverser() {}
	NewTraverser(std::string fileName)
		:m_fileName(fileName){}
	~NewTraverser() {}

public: // member variables
	string m_fileName;											///< This name is used to name the Graphviz file and gif that are produced when cycles are encountered

public: // member fcn
	virtual void Traverse(const Udm::Object &udmObject);

	void FindRootNodes(const CyPhyML::Component &, set<CyPhyML::ValueFlowTarget> &);
	void FindRootNodes(const CyPhyML::TestComponent &, set<CyPhyML::ValueFlowTarget> &);
	void FindRootNodes(const CyPhyML::ComponentAssembly &, set<CyPhyML::ValueFlowTarget> &);
	void FindRootNodes(const CyPhyML::TestBenchType &, set<CyPhyML::ValueFlowTarget> &);
	void FindLeafNodes(const set<CyPhyML::ValueFlowTarget> &, set<CyPhyML::ValueFlowTarget> &);
	void FindLeafNodes(const CyPhyML::ValueFlowTarget &, set<CyPhyML::ValueFlowTarget> &);
	void FindLeafNodes(const CyPhyML::TestBenchType&, set<CyPhyML::ValueFlowTarget> &);				// TODO: 12/20/11 Test Injection Point stuff
	bool IsLeafNode(const CyPhyML::ValueFlowTarget &);											// TODO: 12/20/11 Test Injection Point stuff
	bool IsRootNode(const CyPhyML::ValueFlowTarget &);
	bool CheckValueFormula(const CyPhyML::ValueFormula &);
	bool IsNamedElementStandAlone(const CyPhyML::ValueFlowTarget &);

	//virtual bool EvaluatePPC(CyPhyML::ValueFlowTarget &, double &);
	//virtual bool EvaluateFormula(CyPhyML::ValueFormula &, double &);

	void NamedElements2Process(CyPhyML::ValueFlowTarget&);

	void EvaluateLeafNodes(set<CyPhyML::ValueFlowTarget> &leafNodes);

	// stuff for outputting graph viz files	
	void PrintUnProcessedNamedElements();
	void PrintUnProcessedNamedElements(const CyPhyML::ValueFlowTarget&, set<long>&, map<long, string>&, vector<string>&);
	void PrintGraphOfCycles(map<long, string>&, vector<string>&);	

protected:
	void UpdateNamedElementValue(CyPhyML::ValueFlowTarget &, CyPhyML::ValueFlowTarget &, double);
	void UpdateNamedElementValue(CyPhyML::ValueFlowTarget &vfTarget, double value);
	void UpdateNamedElementValue(CyPhyML::ValueFlowTarget &vfTarget, CyPhyML::unit&, double value);
	void UpdateNamedElementValue(CyPhyML::ValueFlowTarget &vfTarget, CyPhyML::unit&, std::string valueStr);


	set<CyPhyML::ValueFlowTarget> m_allNamedElements2Process;	///< List of all ValueFlowTargets in the model that have not been evaluated
	string m_outPath;											///< Output path of gv and gif files
	map<long, double> m_calculatedValueFlowTargets;		///< Map of all ValueFlowTargets and their calculated value with their GME ID as the key
	map<long, int> m_visitedNodes;						///< Keeps track of ValueFlowTargets (only Parameter, CADParameter, CyberParameter, Property, and Metric) and their state (done visiting, unvisited, currently visiting) to find cycles
	set<CyPhyML::ValueFlowTarget> m_rootNodes;			///< List of VF rootNodes
	Udm::Object m_BoundingBox;
	UnitUtil unitUtil;

protected:
	// Provide a version of this function that uses a cache, and speed up this slow function a bit
	enum idfCacheValue {
		NOT_IN_CACHE = 0,
		NO,
		YES
	};
	map<std::pair<Uml::Class,Uml::Class>,idfCacheValue> idfCache;
	bool IsDerivedFrom(const Uml::Class &derived, const Uml::Class &base) {
		std::pair<Uml::Class,Uml::Class> toCompare(derived,base);
		idfCacheValue icv = idfCache[toCompare];
		switch (icv) {
			case YES:
				return true;
				break;
			case NO:
				return false;
				break;
			default:
				bool result = Uml::IsDerivedFrom(derived,base);
				if (result) {
					idfCache[toCompare] = YES;
					return true;
				} else {
					idfCache[toCompare] = NO;
					return false;
				}
				break;
		}
		return false;
	}


// new stuff - check for unit compatibility
protected:
	map<CyPhyML::ValueFlowTarget,UnitUtil::ValueUnitRep> m_ValueUnitMap;
	map<long, UnitUtil::ValueUnitRep> m_convertedValueFlowTargets_SIMap;


	virtual bool EvaluatePPC(CyPhyML::ValueFlowTarget &, UnitUtil::ValueUnitRep &);
	virtual bool EvaluateFormula(CyPhyML::ValueFormula &, UnitUtil::ValueUnitRep &);

	void PrintNodes(set<CyPhyML::ValueFlowTarget>& leafNodes, string type);


	// 7-3-2012: CADParameter inside CADModel instead of Component
	set<CyPhyML::CADParameter> m_cadParameters;
	set<CyPhyML::ManufacturingModelParameter> m_manufactureParameters;
	set<CyPhyML::ModelicaParameter> m_modelicaParameters; 	// ZL 11/20/2013 support modelica parameters as value flow targets
	set<CyPhyML::Units> m_units_folders;
	map<string, CyPhyML::unit> m_unit_name_table;
	void EvaluateCADParameters();
	void EvaluateManufactureParameters();

	std::string NonRealValueFixture( CyPhyML::ValueFlowTarget &vft, std::string &value );

	void EvaluateModelicaParameters();					// ZL 11/20/2013 support modelica parameters as value flow targets
	CyPhyML::unit FindUnitByName(string);				// 11-29-2012: Added lookup of CyPhyML::Unit by string
	CyPhyML::RootFolder FindTopRootFolder(const Udm::Object &);
	void FindUnitsFolders(const Udm::Object &);
	void FindUnitsFolders(const CyPhyML::RootFolder &);


	// 8-30-2012: PostProcessing generation for Jason
	//string output_dir_;
	//void GeneratePostProcessingScripts(CyPhyML::TestBench &);

	// 2-20-2013: Test Bench Suite
	void EvaluateTestBenchSuite(CyPhyML::TestBenchSuite &);
	string GetVftUnitAndValue(const CyPhyML::ValueFlowTarget&, UnitUtil::ValueUnitRep&);
};


#endif