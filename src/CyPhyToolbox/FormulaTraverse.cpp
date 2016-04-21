#include "FormulaTraverse.h"
#include "EvaluateFormula.h"
//#include "CyPhyElaborate.h"

#include "UdmConsole.h"
#include "UdmApp.h"
#include "UdmUtil.h"
#include "UdmConfig.h"
#include "UdmFormatter.h"

#include "UdmGmeUtil.h"

#include <stdio.h>
#include <direct.h>
#include <string.h>
#include <algorithm>
#include <iterator>


std::wstring wstringFromUTF8(const std::string& utf8);
std::wstring wstringFromUTF8(const Udm::StringAttr& attr);

//extern string OutputDir;

/** \brief Global traverse function that kicks start everything when called in UdmMain. It calls NewTraverse's traverse function.
    \param [in] focusObject Reference to currently opened model in GME
    \param [in] selectedObjects Reference to a set of currently selected objects in GME  
    \return void
*/
void Traverse(const Udm::Object &focusObject)
{
//	GMEConsole::Console::writeLine("Traverse(focusObject, selectedObjects)", MSG_INFO);
	NewTraverser traverser;

	traverser.Traverse(focusObject);
}

/** \brief Global traverse function that kicks start everything when called in UdmMain. It calls NewTraverse's traverse function. 
    \param [in] selectedObjects Reference to a set of currently selected objects in GME
    \return void
*/
void Traverse(const std::set<Udm::Object> &selectedObjects)
{
	GMEConsole::Console::writeLine("Traverse(selectedObjects)", MSG_INFO);
	for (std::set<Udm::Object>::iterator i = selectedObjects.begin(); i != selectedObjects.end(); i++)
	{
		Udm::Object udmObj (*i);
		NewTraverser traverser;
		traverser.Traverse(udmObj);
	}
}


/*//////////////////////////////////////////////////////////////////////////////////////
									New Traverser 
//////////////////////////////////////////////////////////////////////////////////////*/

/** \brief Traverses the passed in udmObject. 
		   1. Finds all the root ValueFlowTarget objects in the udmObject. Root means no incoming ValueFlow connections
		   2. From the root nodes, find all the leaf nodes. Leaf means no outgoing ValueFlowConnections
		   3. If there are no roots or leafs then we might have a cycle and prints out a graphvz file showing the cycle

    \param [in] udmObject Reference to a udm object.
    \return void
*/
void NewTraverser::Traverse(const Udm::Object &udmObject)
{
	m_BoundingBox = udmObject;
	set<CyPhyML::ValueFlowTarget> leafNodes;
	Uml::Class myType = udmObject.type();
	
	if (myType == CyPhyML::TestBenchSuite::meta)
	{
		CyPhyML::TestBenchSuite tbsuite = CyPhyML::TestBenchSuite::Cast(udmObject);
		this->EvaluateTestBenchSuite(tbsuite);
	}
	else
	{
		if (IsDerivedFrom(myType, CyPhyML::TestBenchType::meta))
		{
			CyPhyML::TestBenchType testBench = CyPhyML::TestBenchType::Cast(udmObject);
			this->m_fileName = testBench.name();

			this->FindRootNodes(testBench, m_rootNodes);	// Find Root at TB + SUT	
			this->FindLeafNodes(testBench, leafNodes);		// Find Leaf only at TB level, do not go into SUT etc

			// DY: 10-29-2012
			if (m_rootNodes.size() > 0)
			{
				FindLeafNodes(m_rootNodes, leafNodes);		// Find all Leaf from root. Takes care of cases where there are no leaf at TB level but some inside a SUT
			}
		}
		else
		{
			if (myType == CyPhyML::Component::meta)
			{
				CyPhyML::Component component = CyPhyML::Component::Cast(udmObject);
				this->m_fileName = component.name();
				this->FindRootNodes(component, m_rootNodes);
			}
			else if (myType == CyPhyML::TestComponent::meta)
			{
				CyPhyML::TestComponent testComponent = CyPhyML::TestComponent::Cast(udmObject);
				this->m_fileName = testComponent.name();
				this->FindRootNodes(testComponent, m_rootNodes);
			}
			else if (myType == CyPhyML::ComponentAssembly::meta)
			{
				CyPhyML::ComponentAssembly assembly = CyPhyML::ComponentAssembly::Cast(udmObject);
				this->m_fileName = assembly.name();
				this->FindRootNodes(assembly, m_rootNodes);
			}

			if (!m_rootNodes.empty() || !m_cadParameters.empty() || !m_manufactureParameters.empty() || !m_modelicaParameters.empty() || !m_carParameters.empty())
			{
				FindLeafNodes(m_rootNodes, leafNodes);
			//	PrintNodes(m_rootNodes, "Root");
			//	PrintNodes(leafNodes, "Leaf");	
		
			
			//	EvaluateLeafNodes(leafNodes);
		#ifdef GRAPHVIZ
				PrintUnProcessedNamedElements();
		#endif
			}
			else
			{
				GMEConsole::Console::writeLine("FormulaEvaluator - 0 ValueFlow objects found to start evaluation from.", MSG_INFO);
				PrintUnProcessedNamedElements();
			}
		}
	
		FindUnitsFolders(udmObject);
		EvaluateLeafNodes(leafNodes);
		EvaluateCADParameters();
		EvaluateManufactureParameters();
		EvaluateModelicaParameters();
		EvaluateCarParameters();
	}

}

/** \brief Find all root nodes (no incoming ValueFlow connections) and sticks them in nodes for a cyphy component.
    \param [in] component Reference to a cyphy component object which will be traversed to find root nodes.
	\param [in, out] nodes Reference to a set of ValueFlowTarget, this will store all root nodes found.
    \return void
*/
void NewTraverser::FindRootNodes(const CyPhyML::Component &component, set<CyPhyML::ValueFlowTarget> &nodes)
{
	set<CyPhyML::ValueFlowTarget> vft_Set = component.ValueFlowTarget_kind_children();
	for (set<CyPhyML::ValueFlowTarget>::iterator i = vft_Set.begin(); i != vft_Set.end(); i++)
	{
		CyPhyML::ValueFlowTarget vft(*i);
		string name = vft.name();
		if (this->IsRootNode(vft))
			nodes.insert(vft);
	}

	// *************************
	set<CyPhyML::CarModel> carModel_Set = component.CarModel_kind_children();
	for (set<CyPhyML::CarModel>::const_iterator ci = carModel_Set.begin(); ci != carModel_Set.end(); ci++)
	{
		set<CyPhyML::CarParameter> carParam = ci->CarParameter_kind_children();
		for (set<CyPhyML::CarParameter>::const_iterator di = carParam.begin(); di != carParam.end(); di++)
			m_carParameters.insert(*di);
	}
	// *************************


	set<CyPhyML::CADModel> cadModel_Set = component.CADModel_kind_children();
	for (set<CyPhyML::CADModel>::const_iterator ci = cadModel_Set.begin(); ci != cadModel_Set.end(); ci++)
	{
		set<CyPhyML::CADParameter> cadParam = ci->CADParameter_kind_children();
		for (set<CyPhyML::CADParameter>::const_iterator di = cadParam.begin(); di != cadParam.end(); di++)
			m_cadParameters.insert(*di);
	}

	set<CyPhyML::ManufacturingModel> manModel_set = component.ManufacturingModel_kind_children();
	for (set<CyPhyML::ManufacturingModel>::const_iterator ci = manModel_set.begin(); ci != manModel_set.end(); ci++)
	{
		set<CyPhyML::ManufacturingModelParameter> param = ci->ManufacturingModelParameter_kind_children();
		for (set<CyPhyML::ManufacturingModelParameter>::const_iterator di = param.begin(); di != param.end(); di++)
			m_manufactureParameters.insert(*di);
	}

	// ZL 11/20/2013 support modelica parameters as value flow targets
	set<CyPhyML::ModelicaModelType> modelicaModel_set = component.ModelicaModelType_kind_children();
	for (set<CyPhyML::ModelicaModelType>::const_iterator ci = modelicaModel_set.begin(); ci != modelicaModel_set.end(); ci++)
	{
		set<CyPhyML::ModelicaParameter> param = ci->ModelicaParameter_kind_children();
		for (set<CyPhyML::ModelicaParameter>::const_iterator di = param.begin(); di != param.end(); di++)
		{
			m_modelicaParameters.insert(*di);
		}
	}

	set<CyPhyML::ModelicaConnector> modelicaConnector_set = component.ModelicaConnector_kind_children();
	for (set<CyPhyML::ModelicaConnector>::const_iterator ci = modelicaConnector_set.begin(); ci != modelicaConnector_set.end(); ci++)
	{
		set<CyPhyML::ModelicaParameter> param = ci->ModelicaParameter_kind_children();
		for (set<CyPhyML::ModelicaParameter>::const_iterator di = param.begin(); di != param.end(); di++)
		{
			m_modelicaParameters.insert(*di);
		}
	}
}

	// ZL 11/20/2013 support modelica parameters as value flow targets
void NewTraverser::FindRootNodes(const CyPhyML::TestComponent &component, set<CyPhyML::ValueFlowTarget> &nodes)
{
	set<CyPhyML::ValueFlowTarget> vft_Set = component.ValueFlowTarget_kind_children();
	for (set<CyPhyML::ValueFlowTarget>::iterator i = vft_Set.begin(); i != vft_Set.end(); i++)
	{
		CyPhyML::ValueFlowTarget vft(*i);
		string name = vft.name();
		if (this->IsRootNode(vft))
			nodes.insert(vft);
	}

	set<CyPhyML::CADModel> cadModel_Set = component.CADModel_kind_children();
	for (set<CyPhyML::CADModel>::const_iterator ci = cadModel_Set.begin(); ci != cadModel_Set.end(); ci++)
	{
		set<CyPhyML::CADParameter> cadParam = ci->CADParameter_kind_children();
		for (set<CyPhyML::CADParameter>::const_iterator di = cadParam.begin(); di != cadParam.end(); di++)
			m_cadParameters.insert(*di);
	}

	set<CyPhyML::ManufacturingModel> manModel_set = component.ManufacturingModel_kind_children();
	for (set<CyPhyML::ManufacturingModel>::const_iterator ci = manModel_set.begin(); ci != manModel_set.end(); ci++)
	{
		set<CyPhyML::ManufacturingModelParameter> param = ci->ManufacturingModelParameter_kind_children();
		for (set<CyPhyML::ManufacturingModelParameter>::const_iterator di = param.begin(); di != param.end(); di++)
			m_manufactureParameters.insert(*di);
	}

	set<CyPhyML::ModelicaModelType> modelicaModel_set = component.ModelicaModelType_kind_children();
	for (set<CyPhyML::ModelicaModelType>::const_iterator ci = modelicaModel_set.begin(); ci != modelicaModel_set.end(); ci++)
	{
		set<CyPhyML::ModelicaParameter> param = ci->ModelicaParameter_kind_children();
		for (set<CyPhyML::ModelicaParameter>::const_iterator di = param.begin(); di != param.end(); di++)
		{
			m_modelicaParameters.insert(*di);
		}
	}

	set<CyPhyML::ModelicaConnector> modelicaConnector_set = component.ModelicaConnector_kind_children();
	for (set<CyPhyML::ModelicaConnector>::const_iterator ci = modelicaConnector_set.begin(); ci != modelicaConnector_set.end(); ci++)
	{
		set<CyPhyML::ModelicaParameter> param = ci->ModelicaParameter_kind_children();
		for (set<CyPhyML::ModelicaParameter>::const_iterator di = param.begin(); di != param.end(); di++)
		{
			m_modelicaParameters.insert(*di);
		}
	}
}

/** \brief Find all root nodes (no incoming ValueFlow connections) and sticks them in nodes for a cyphy component assembly.
    \param [in] componentAssembly Reference to a cyphy component assembly object which will be traversed to find root nodes.
	\param [in, out] nodes Reference to a set of ValueFlowTarget, this will store all root nodes found.
    \return void
*/
void NewTraverser::FindRootNodes(const CyPhyML::ComponentAssembly &componentAssembly, set<CyPhyML::ValueFlowTarget> &nodes)
{
	set<CyPhyML::Component> c_Set = componentAssembly.Component_kind_children();
	for (set<CyPhyML::Component>::iterator i = c_Set.begin(); i != c_Set.end(); i++)
	{
		CyPhyML::Component component(*i);
		FindRootNodes(component, nodes);
	}

	set<CyPhyML::ComponentAssembly> ca_Set = componentAssembly.ComponentAssembly_kind_children();
	for (set<CyPhyML::ComponentAssembly>::iterator i = ca_Set.begin(); i != ca_Set.end(); i++)
	{
		CyPhyML::ComponentAssembly componentAssembly(*i);
		FindRootNodes(componentAssembly, nodes);
	}

	set<CyPhyML::ValueFlowTarget> vtf_Set = componentAssembly.ValueFlowTarget_kind_children();
	for (set<CyPhyML::ValueFlowTarget>::iterator i = vtf_Set.begin(); i != vtf_Set.end(); i++)
	{
		CyPhyML::ValueFlowTarget vft(*i);
		if (this->IsRootNode(vft))
			nodes.insert(vft);
	}

	set<CyPhyML::ModelicaConnector> modelicaConnector_set = componentAssembly.ModelicaConnector_kind_children();
	for (set<CyPhyML::ModelicaConnector>::const_iterator ci = modelicaConnector_set.begin(); ci != modelicaConnector_set.end(); ci++)
	{
		set<CyPhyML::ModelicaParameter> param = ci->ModelicaParameter_kind_children();
		for (set<CyPhyML::ModelicaParameter>::const_iterator di = param.begin(); di != param.end(); di++)
		{
			m_modelicaParameters.insert(*di);
		}
	}
}

/** \brief Find all root nodes (no incoming ValueFlow connections) and sticks them in nodes for a cyphy test bench.
    \param [in] testBench Reference to a cyphy test bench object which will be traversed to find root nodes.
	\param [in, out] nodes Reference to a set of ValueFlowTarget, this will store all root nodes found.
    \return void
*/
void NewTraverser::FindRootNodes(const CyPhyML::TestBenchType &testBench, set<CyPhyML::ValueFlowTarget> &nodes)
{	
	set<CyPhyML::Component> c_Set = testBench.Component_kind_children();
	for (set<CyPhyML::Component>::iterator i = c_Set.begin(); i != c_Set.end(); i++)
	{
		CyPhyML::Component component(*i);
		FindRootNodes(component, nodes);
	}

	set<CyPhyML::TestComponent> tc_Set = testBench.TestComponent_kind_children();
	for (set<CyPhyML::TestComponent>::iterator i = tc_Set.begin(); i != tc_Set.end(); i++)
	{
		CyPhyML::TestComponent testComponent(*i);
		FindRootNodes(testComponent, nodes);
	}

	set<CyPhyML::ComponentAssembly> ca_Set = testBench.ComponentAssembly_kind_children();
	for (set<CyPhyML::ComponentAssembly>::iterator i = ca_Set.begin(); i != ca_Set.end(); i++)
	{
		CyPhyML::ComponentAssembly componentAssembly(*i);
		FindRootNodes(componentAssembly, nodes);
	}

	set<CyPhyML::ValueFlowTarget> vtf_Set = testBench.ValueFlowTarget_kind_children();
	for (set<CyPhyML::ValueFlowTarget>::iterator i = vtf_Set.begin(); i != vtf_Set.end(); i++)
	{
		CyPhyML::ValueFlowTarget vft(*i);
		if (this->IsRootNode(vft))
			nodes.insert(vft);
	}
}

/** \brief Checks if a ValueFormula can be evaluated meaning it has at least 1 incoming and 1 outgoing ValueFlow connections.
    \param [in] formula Reference to a ValueFormula object to be evaluated.
    \return void
*/
bool NewTraverser::CheckValueFormula(const CyPhyML::ValueFormula &formula)
{
	GMEConsole::Console::writeLine("Hellow", MSG_WARNING);
	bool stat = 1;
	set<CyPhyML::ValueFlow> vf_Src_Set = formula.srcValueFlow();
	set<CyPhyML::ValueFlow> vf_Dst_Set = formula.dstValueFlow();
	if (vf_Src_Set.size() == 0)
	{
		string message = "FormulaEvaluator -Formula [" + UdmGme::UdmId2GmeId(formula.uniqueId()) + "] does not have valid inputs so it can not be evaluated.";
		GMEConsole::Console::writeLine(message, MSG_ERROR);
		throw udm_exception (message);
			
		stat = 0;
	}
	
	if (vf_Dst_Set.size() == 0)
	{
		GMEConsole::Console::writeLine("FormulaEvaluator -Formula [" + UdmGme::UdmId2GmeId(formula.uniqueId()) + "] does not have outputs.", MSG_WARNING);
		stat = 0;
	}

	return stat;
}

/** \brief Checks whether a NamedElement object has any ValueFlow connections
    \param [in] vft Reference to a cyphy ValueFlowTarget object to be evaluated.
    \return 1 if it has no ValueFlow connections, 0 otherwise
*/
bool NewTraverser::IsNamedElementStandAlone(const CyPhyML::ValueFlowTarget &vft)
{
	set<CyPhyML::ValueFlow> vf_Src_Set = vft.srcValueFlow();
	set<CyPhyML::ValueFlow> vf_Dst_Set = vft.dstValueFlow();	
	return (vf_Src_Set.size() == 0 && vf_Dst_Set.size() == 0);
}

/** \brief Find all leaf nodes (no incoming ValueFlow connections) from root nodes.
    \param [in] rootNodes Set of root ValueFlowTarget nodes to be evaluated.
	\param [in, out] leafNodes Stores reference to leaf ValueFlowTarget found.
    \return void
*/
void NewTraverser::FindLeafNodes(const set<CyPhyML::ValueFlowTarget> &rootNodes, set<CyPhyML::ValueFlowTarget> &leafNodes)
{

	for (set<CyPhyML::ValueFlowTarget>::iterator i = rootNodes.begin(); i != rootNodes.end(); i++)
	{
		CyPhyML::ValueFlowTarget vft(*i);	
		string path = vft.getPath2("/", false);
		this->m_visitedNodes[vft.uniqueId()] = 0;
		FindLeafNodes(vft, leafNodes);
	}
}

/** \brief Find all leaf nodes (no incoming ValueFlow connections) for 1 root node.
		   During the process also checks for cycles by doing depth first traversal by using m_visitedNodes and the following code.

		   0: new
		   1: visting
		   2: finished

    \param [in] rootNode A root ValueFlowTarget node to be evaluated.
	\param [in, out] leafNodes Stores reference to leaf ValueFlowTarget found.
    \return void
*/
void NewTraverser::FindLeafNodes(const CyPhyML::ValueFlowTarget &rootNode, set<CyPhyML::ValueFlowTarget> &leafNodes)
{	string path = rootNode.getPath2("/", false);
	bool stat = 1;
	Udm::Object myParent = rootNode.GetParent();
	map<long, int>::iterator i = this->m_visitedNodes.find(rootNode.uniqueId());
	if (i != this->m_visitedNodes.end())
		this->m_visitedNodes[rootNode.uniqueId()] = 1;

	set<CyPhyML::ValueFlow> dstVF_Set = rootNode.dstValueFlow();
	for (set<CyPhyML::ValueFlow>::iterator i = dstVF_Set.begin(); i != dstVF_Set.end(); i++)
	{
		CyPhyML::ValueFlow dst_vf(*i);
		CyPhyML::ValueFlowTarget dst_vfTarget = dst_vf.dstValueFlow_end();
		if (dst_vfTarget != Udm::null)
			string outgoing_vft_path = dst_vfTarget.getPath2("/", false);
		else
			string outgoing_vft_path = "I am null";
		//	DY 11/22/11: Do not follow ValueFlow outside of Bounding Box
		Udm::Object dst_vf_Parent = dst_vf.GetParent();
		if (myParent == m_BoundingBox && dst_vf_Parent != m_BoundingBox)
			continue;
		//

		map<long, int>::iterator  j = this->m_visitedNodes.find(dst_vfTarget.uniqueId());
		if (j != this->m_visitedNodes.end())
		{
			if (j->second == 0)
			{
				//if (!this->FindCyles(src_vfTarget))
				//	return false;
				this->FindLeafNodes(dst_vfTarget, leafNodes);
				//	result = 0;
			}
			else if (j->second == 1)
			{
				GMEConsole::Console::writeLine("CYCLE DETECTED {", MSG_ERROR);
				GMEConsole::Console::writeLine("	Node [" + (std::string)rootNode.name() + "," + rootNode.getPath2("/", false) + "] --> Node[" + (std::string)dst_vfTarget.name() + "," + dst_vfTarget.getPath2("/", false) + "]", MSG_ERROR);
				GMEConsole::Console::writeLine("}", MSG_ERROR);
				throw udm_exception ("FormulaEvaluator -Cycle Detected in value flow, " + rootNode.getPath2("/", false) + " --> " + dst_vfTarget.getPath2("/", false));
				//result = 0;			
			}
			else if (j->second == 2)
			{
				continue;
			}
			else
				GMEConsole::Console::writeLine("Unknown color!", MSG_WARNING);
		}	
		else
		{
			this->m_visitedNodes[dst_vfTarget.uniqueId()] = 0;
			//if (!this->FindCyles(src_vfTarget))
			//	return false;
			this->FindLeafNodes(dst_vfTarget, leafNodes);
		}

	} // end for

	if (dstVF_Set.size() == 0)               // no outgoing ValueFlow
		leafNodes.insert(rootNode);

	i->second = 2;
}


void NewTraverser::FindLeafNodes(const CyPhyML::TestBenchType& testBench, set<CyPhyML::ValueFlowTarget> &leafNodes)
{
	set<CyPhyML::ValueFlowTarget> vtf_Set = testBench.ValueFlowTarget_kind_children();
	for (set<CyPhyML::ValueFlowTarget>::iterator i = vtf_Set.begin(); i != vtf_Set.end(); i++)
	{
		CyPhyML::ValueFlowTarget vft(*i);
		if (this->IsLeafNode(vft))
			leafNodes.insert(vft);
	}
}

static std::set<CyPhyML::ValueFlow> RemoveVFsWithDstRefports(std::set<CyPhyML::ValueFlow> vfs)
{
	for (auto it = vfs.begin(); it != vfs.end();)
	{
		if (IsDestRefport(*it))
			vfs.erase(it++);
		else
			++it;
	}
	return vfs;
}

/** \brief Checks if a ValueFlowTarget is a root node.

	 IN conns | Out conns |
   ------------------------------------------
	  0       |  0		  | standalone
	  0       |  1		  | root, rootList
	  1		  |  0		  | allList
	  1		  |  1		  | allList	  

	\param [in, out] vft Reference to a ValueFlowTarget to be evaluated.
    \return 1 if vft is a root node, 0 otherwise
*/
bool NewTraverser::IsRootNode(const CyPhyML::ValueFlowTarget &vft)
{
	bool isRoot = 0;
	Udm::Object vftParent = vft.GetParent();

	//	DY 11/22/11: Modified to respect Bounding Box
	if (   IsDerivedFrom(vft.type(), CyPhyML::HasDescriptionAndGUID::meta)
		|| IsDerivedFrom(vft.type(), CyPhyML::Constant::meta))
	{
		bool hasIn = 0, hasOut = 0;
		set<CyPhyML::ValueFlow> srcVFs = RemoveVFsWithDstRefports(vft.srcValueFlow());			// in VFs
		set<CyPhyML::ValueFlow> dstVFs = vft.dstValueFlow();			// out VFs
		string tmp = vft.name();

		if (srcVFs.size() > 1)
		{
			string message = "FormulaEvaluator -" + vft.getPath2("/", false) + " has >1 incoming VFs!";
			GMEConsole::Console::writeLine(message, MSG_ERROR);
			throw udm_exception (message);
		}

		if (vftParent == m_BoundingBox)
		{
			// valid in VFs ?
			for (set<CyPhyML::ValueFlow>::const_iterator ci = srcVFs.begin(); ci != srcVFs.end(); ci++)
			{
				if (ci->GetParent() == m_BoundingBox)
				{
					hasIn = 1;	break;
				}
			}

			// valid out VFs ?
			for (set<CyPhyML::ValueFlow>::const_iterator ci = dstVFs.begin(); ci != dstVFs.end(); ci++)
			{
				if (ci->GetParent() == m_BoundingBox)
				{
					hasOut = 1;	break;
				}
			}
		}
		else
		{
			hasIn = srcVFs.size() > 0;
			hasOut = dstVFs.size() > 0;
		}

		if (hasIn || hasOut)
		{
			this->m_allNamedElements2Process.insert(vft);
			
			if (!hasIn)
				isRoot = 1;
		}
	}

	return isRoot;
}

bool NewTraverser::IsLeafNode(const CyPhyML::ValueFlowTarget &vft)
{
	int outCount = 0;
	Udm::Object vftParent = vft.GetParent();

	string name = vft.name();

	if (   IsDerivedFrom(vft.type(), CyPhyML::HasDescriptionAndGUID::meta)
		|| IsDerivedFrom(vft.type(), CyPhyML::Constant::meta))
	{
		set<CyPhyML::ValueFlow> srcVFs = vft.srcValueFlow();			// in VFs
		set<CyPhyML::ValueFlow> dstVFs = vft.dstValueFlow();			// out VFs

		if (srcVFs.size() > 1)
		{
			string message = "FormulaEvaluator -" + vft.getPath2("/", false) + "has >1 incoming VFs!";
			GMEConsole::Console::writeLine(message, MSG_ERROR);
			throw udm_exception (message);
		}

		if (!dstVFs.empty())
		{
			if (vftParent == m_BoundingBox)
			{
				// valid out VFs ?
				for (set<CyPhyML::ValueFlow>::const_iterator ci = dstVFs.begin(); ci != dstVFs.end(); ci++)
				{
					if (ci->GetParent() == m_BoundingBox)
						outCount++;
				}
			}
			else
				outCount = dstVFs.size();
		}
	}

	return (outCount == 0);
}

#if 0
/** \brief Evalues Named Element objects (Parameter, CADParameter, CyberParameter, Property, Metrics).
    \param [in] vf Reference to a ValueFlowTarget to be evaluated.
	\param [in, out] result Result of evaluating the ValueFlowTarget
    \return 1 if evaluation was successful, 0 otherwise
*/
bool NewTraverser::EvaluatePPC(CyPhyML::ValueFlowTarget &vf, double &result)
{
	std::string name = vf.name(), id = UdmGme::UdmId2GmeId(vf.uniqueId());

	bool stat = 1;
	std::string val;

	//  :::already evaluated :::
	map<long, double>::const_iterator ci = m_calculatedValueFlowTargets.find(vf.uniqueId());
	if (ci != m_calculatedValueFlowTargets.end())
	{
		result = ci->second;
		return 1;
	}

	// ::: not evaluated yet :::	
	// DY 11/23/11: check for root node
	bool isRoot = (m_rootNodes.find(vf) != m_rootNodes.end());
	set<CyPhyML::ValueFlow> VF_Set = vf.srcValueFlow();
	if (VF_Set.size() > 0 && !isRoot)		 
	{
		double tmp = 0;

		if (VF_Set.size() > 1)
			GMEConsole::Console::writeLine((std::string)vf.name() + "[" + UdmGme::UdmId2GmeId(vf.uniqueId()) + "]"+ "has >1 incoming ValueFlow connections. Picking the first one from the list returned by Udm.", MSG_ERROR);

		CyPhyML::ValueFlowTarget src_vfTarget = (CyPhyML::ValueFlow(*VF_Set.begin())).srcValueFlow_end();
		
		if (IsDerivedFrom(src_vfTarget.type(), CyPhyML::HasDescriptionAndGUID::meta))
		{
			if (!this->EvaluatePPC(src_vfTarget, tmp))
				stat = 0;
			else  				
				UpdateNamedElementValue(src_vfTarget, vf, tmp);		// calls FormulaTraverse's UpdateNamedElementValue() to update value in the model
			
			result = tmp;
			m_calculatedValueFlowTargets[vf.uniqueId()] = result;
			m_allNamedElements2Process.erase(vf); //NamedElements2Process(vf);

		}
		else if (IsDerivedFrom(src_vfTarget.type(), CyPhyML::ValueFormula::meta))
		{
			if (!this->EvaluateFormula(CyPhyML::ValueFormula::Cast(src_vfTarget), tmp))
				stat = 0;
			else
				UpdateNamedElementValue(vf, tmp);		// update in model

			result = tmp;
			m_calculatedValueFlowTargets[vf.uniqueId()] = result;
			m_allNamedElements2Process.erase(vf); //NamedElements2Process(vf);			
		}
	}
	else	// else: standalone ValueFlowTarget, use the val variable from the beginning of this function
	{	
		// getting defaults
		if (vf.type() == CyPhyML::Parameter::meta)
			val = CyPhyML::Parameter::Cast(vf).Value();
		else if (vf.type() == CyPhyML::CADParameter::meta)
			val = CyPhyML::CADParameter::Cast(vf).Value();
		else if (vf.type() == CyPhyML::Property::meta)
			val = CyPhyML::Property::Cast(vf).Value();
		else if (vf.type() == CyPhyML::Metric::meta)
			val = CyPhyML::Metric::Cast(vf).Value();
		else
		{
			GMEConsole::Console::writeLine((std::string)vf.name() + " is not Parameter, CADParameter, CyberParameter, Metrics or Property! Skipping!", MSG_INFO);
			stat = 0;
		}

		if (val == "")
			val = "0";
		result = std::atof(val.c_str());
		m_calculatedValueFlowTargets[vf.uniqueId()] = result;
		m_allNamedElements2Process.erase(vf); //NamedElements2Process(vf);
	}

	// Set my value to the calculated

	return stat;
}


/** \brief Evalues Value Formula objects (Simple Formula, Custom Formula).
    \param [in] vf Reference to a ValueFlowTarget to be evaluated.
	\param [in, out] result Result of evaluating the ValueFlowTarget
    \return 1 if evaluation was successful, 0 otherwise
*/
bool NewTraverser::EvaluateFormula(CyPhyML::ValueFormula &vf, double &result)
{
	std::string name = vf.name(), id = UdmGme::UdmId2GmeId(vf.uniqueId());
	bool simpleFormula = (vf.type() == CyPhyML::SimpleFormula::meta);
	std::multimap<std::string, double> parameters;

	//  :::already evaluated :::
	map<long, double>::const_iterator ci = m_calculatedValueFlowTargets.find(vf.uniqueId());
	if (ci != m_calculatedValueFlowTargets.end())       // already evaluated before
	{
		result = ci->second;
		return 1;
	}

	//  :::not evaluated yet :::
	set<CyPhyML::ValueFlow> VF_Set = vf.srcValueFlow();
	if (VF_Set.size() == 0)
	{
		string id = UdmGme::UdmId2GmeId(vf.uniqueId());
		GMEConsole::Console::writeLine("Can not evaluate Fomula [" + GMEConsole::Formatter::MakeObjectHyperlink(UdmGme::UdmId2GmeId(vf.uniqueId()), vf) + "] because it has 0 inputs.", MSG_ERROR);
		return 0;
	}

	for (set<CyPhyML::ValueFlow>::iterator i = VF_Set.begin(); i != VF_Set.end(); i++)
	{
		double tmp = 0;

		CyPhyML::ValueFlow src_vf(*i);
		CyPhyML::ValueFlowTarget src_vfTarget = src_vf.srcValueFlow_end();
		std::string vf_variableName = src_vf.FormulaVariableName();

		if (IsDerivedFrom(src_vfTarget.type(), CyPhyML::HasDescriptionAndGUID::meta))
		{
			if (this->EvaluatePPC(src_vfTarget, tmp))
				parameters.insert(pair<std::string, double>((vf_variableName == "") ? src_vfTarget.name() : vf_variableName, tmp));
		}
		else if (IsDerivedFrom(src_vfTarget.type(), CyPhyML::ValueFormula::meta))              // formula connected to formula, need to use the FormulaVariableName attribute from the connection 
		{
			if (this->EvaluateFormula(CyPhyML::ValueFormula::Cast(src_vfTarget), tmp))
			{
				if (vf_variableName != "")
					parameters.insert(pair<std::string, double>(vf_variableName, tmp));
				else
				{
					GMEConsole::Console::writeLine("Missing VariableName on Formula-2-Formula connection. [" + GMEConsole::Formatter::MakeObjectHyperlink(UdmGme::UdmId2GmeId(vf.uniqueId()), vf) + "," + GMEConsole::Formatter::MakeObjectHyperlink(UdmGme::UdmId2GmeId(src_vfTarget.uniqueId()), src_vfTarget) + "] Using \"Formula\" for calculation.", MSG_WARNING);
					parameters.insert(pair<std::string, double>("Formula", tmp));
				}
			}
		}
	}
	
	if (parameters.size() > 0)
	{
		if (simpleFormula)
			result = EvaluateSimpleFormula(parameters, CyPhyML::SimpleFormula::Cast(vf).Method());
		else
			result = EvaluateCustomFormula(parameters, CyPhyML::CustomFormula::Cast(vf).Expression());

		//Cleanup(parameters);
	}
	else
		return 0;
	
	return 1;
}

#endif

void NewTraverser::NamedElements2Process(CyPhyML::ValueFlowTarget& vft)
{
	set<CyPhyML::ValueFlowTarget>::iterator i = m_allNamedElements2Process.find(vft);
}

/** \brief Generates the graphviz gv file and calls dot to create a gif from the gv file.
    \param [in] nodes A list of ValueFlowTargets' ids involved in cycles
	\param [in] conns A list of connections between ValueFlowTargets involved in cycles
    \return void
*/
void NewTraverser::PrintGraphOfCycles(map<long, string>& nodes, vector<string>& conns)
{		
	string graphviz_fn = this->m_fileName + "_graphviz.gv";
	ofstream graphviz(graphviz_fn.c_str());
	graphviz << "digraph g {\n";
	graphviz << "rankdir=LR\n";
	for (map<long, string>::const_iterator ci = nodes.begin(); ci != nodes.end(); ci++)
	{
		//100004853 [label="Puma_Track_Pkg\n100004853"];
		std::string type, name;
		type = ci->second.substr(0, ci->second.find(":"));
		name = ci->second.substr(ci->second.find(":")+1);
		graphviz<<ci->first<< " ";
		if (type == "NamedElement")          // ellipse
			graphviz<<"[label=\""<<name<<"\\n"<<UdmGme::UdmId2GmeId(ci->first)<<"\",shape=ellipse];\n";
		else                                       // box
			graphviz<<"[label=\""<<name<<"\\n"<<UdmGme::UdmId2GmeId(ci->first)<<"\",shape=box];\n";
	}

	for (vector<string>::const_iterator ci = conns.begin(); ci != conns.end(); ci++)
		graphviz<<*ci<<";\n";
	
	graphviz << "}";
	graphviz.close();	

	char path[255];
	_getcwd(path, 255);
	std::string entry = "dot -Tgif -O \""+ graphviz_fn + "\"";
	system(entry.c_str());
	string url = "file:///" + (std::string)path + "\\" + graphviz_fn + ".gif";
	GMEConsole::Console::writeLine("Generated Graph of cycles: " + GMEConsole::Formatter::MakeHyperlink(url, url), MSG_ERROR);
}

/** \brief Prints unprocessed ValueFlowTargets and the cycle(s) they are involved in.
		   Checks to see if there are unprocessed ValueFlowTargets, unprocessed indicates cycle(s).
		   Traces unprocessed to gather data on cycles to be printed.

    \return void
*/
void NewTraverser::PrintUnProcessedNamedElements()
{
	set<long> visitedValueFlows;
	vector<string> conns;
	map<long, string> nodes; 

	bool hasUnProcessed = 0;

	if (m_allNamedElements2Process.size() > 0)
	{
		hasUnProcessed = 1;
		string unprocessedList;
		for (set<CyPhyML::ValueFlowTarget>::const_iterator i = m_allNamedElements2Process.begin(); i != m_allNamedElements2Process.end(); i++)
		{		
			CyPhyML::ValueFlowTarget target(*i);
			unprocessedList += target.getPath2("/", false) + "|";
			// this->PrintUnProcessedNamedElements(target, visitedValueFlows, nodes, conns);
		}

		string message = "FormulaEvaluator -Nodes with cyclic dependencies [" + unprocessedList + "]";
		GMEConsole::Console::writeLine(message, MSG_ERROR);
		throw udm_exception (message);
	}

	if (hasUnProcessed)
		this->PrintGraphOfCycles(nodes, conns);
}

/** \brief Takes 1 unprocessed ValueFlowTarget and the cycle(s) it is involved in.
		   Checks to see if there are unprocessed ValueFlowTargets, unprocessed indicates cycle(s).
		   Traces unprocessed to gather data on cycles to be printed.

    \return void
*/
void NewTraverser::PrintUnProcessedNamedElements(const CyPhyML::ValueFlowTarget& vft, set<long>& visitedValueFlows, map<long, string>& nodeStream, vector<string>& connStream)
{
	long id = vft.uniqueId();
	string vft_id;
	to_string(vft_id, id);
	if (nodeStream.find(id) == nodeStream.end())
	{
		if (IsDerivedFrom(vft.type(), CyPhyML::ValueFormula::meta))
			nodeStream[id] = "ValueFormula:" + (string)vft.name();
		else
			nodeStream[id] = "NamedElement:" + (string)vft.name();

		set<CyPhyML::ValueFlow> vf_Dst_Set = vft.dstValueFlow();
		for (set<CyPhyML::ValueFlow>::const_iterator ci = vf_Dst_Set.begin(); ci != vf_Dst_Set.end(); ci++)
		{
			CyPhyML::ValueFlow vf(*ci);
			if (visitedValueFlows.find(vf.uniqueId()) == visitedValueFlows.end())       // new ValueFlow
			{
				CyPhyML::ValueFlowTarget dst_vft = vf.dstValueFlow_end();
				string dst_vft_id;
				to_string(dst_vft_id, dst_vft.uniqueId());
				connStream.push_back(vft_id + " -> " + dst_vft_id);
				visitedValueFlows.insert(vf.uniqueId());			
				this->PrintUnProcessedNamedElements(dst_vft, visitedValueFlows, nodeStream, connStream);
			}
		}	
	}
}

void NewTraverser::UpdateNamedElementValue(CyPhyML::ValueFlowTarget &src_vfTarget, CyPhyML::ValueFlowTarget &dst_vfTarget, double value)
{
	std::string valueStr;
	to_string(valueStr, value);

	CyPhyML::ParamPropTarget unitRef;

	if (IsDerivedFrom(src_vfTarget.type(), CyPhyML::Property::meta))
	{
		unitRef = CyPhyML::Property::Cast(src_vfTarget).ref();
	}
	else if (IsDerivedFrom(src_vfTarget.type(), CyPhyML::Parameter::meta))
	{
		unitRef = CyPhyML::Parameter::Cast(src_vfTarget).ref();
	}
	else if (IsDerivedFrom(src_vfTarget.type(), CyPhyML::Metric::meta))
	{
		unitRef = CyPhyML::Metric::Cast(src_vfTarget).ref();
	}


	if (IsDerivedFrom(dst_vfTarget.type(), CyPhyML::Property::meta))
	{
		CyPhyML::Property prop = CyPhyML::Property::Cast(dst_vfTarget);
		prop.Value() = valueStr;

		if (unitRef != Udm::null)
			prop.ref() = unitRef;
	}
	else if (IsDerivedFrom(dst_vfTarget.type(), CyPhyML::Parameter::meta))
	{
		CyPhyML::Parameter param = CyPhyML::Parameter::Cast(dst_vfTarget);
		param.Value() = valueStr;

		if (unitRef != Udm::null)
			param.ref() = unitRef;
	}
	else if (IsDerivedFrom(dst_vfTarget.type(), CyPhyML::Metric::meta))
	{
		CyPhyML::Metric metric = CyPhyML::Metric::Cast(dst_vfTarget);
		metric.Value() = valueStr;

		if (unitRef != Udm::null)
			metric.ref() = unitRef;
	}
	else
	{
		string message = "FormulaEvaluator -Only values of Property, Parameter, Metric, CADParameter, or CyberParameter can be set [" + dst_vfTarget.getPath2("/", false) + "]";
		GMEConsole::Console::writeLine(message, MSG_ERROR);
		throw udm_exception (message);
	}
}

void Cleanup(multimap<std::string, double*> &parameters)
{
	for (multimap<std::string, double*>::iterator i = parameters.begin(); i != parameters.end(); i++)
	{
		delete i->second;
		i->second = 0;
	}
	parameters.clear();
}

void NewTraverser::UpdateNamedElementValue(CyPhyML::ValueFlowTarget &vfTarget, double value)
{
	std::string valueStr;
	to_string(valueStr, value);
	if (vfTarget.type() == CyPhyML::Property::meta)
		CyPhyML::Property::Cast(vfTarget).Value() = valueStr;
	else if (vfTarget.type() == CyPhyML::Parameter::meta)
		CyPhyML::Parameter::Cast(vfTarget).Value() = valueStr;
	else if (vfTarget.type() == CyPhyML::CADParameter::meta)
		CyPhyML::CADParameter::Cast(vfTarget).Value() = valueStr;
	else if (vfTarget.type() == CyPhyML::Metric::meta)
		CyPhyML::Metric::Cast(vfTarget).Value() = valueStr;
	else
	{
		string message = "FormulaEvaluator -Only values of Property, Parameter, Metric, CADParameter, or CyberParameter can be set [" + vfTarget.getPath2("/", false) + "]";
		GMEConsole::Console::writeLine(message, MSG_ERROR);
		throw udm_exception (message);
	}
}

void NewTraverser::UpdateNamedElementValue(CyPhyML::ValueFlowTarget &vfTarget, CyPhyML::unit& unitRef, double value)
{
	std::string valueStr;
	to_string(valueStr, value);
	UpdateNamedElementValue(vfTarget, unitRef, valueStr);
}

void NewTraverser::UpdateNamedElementValue(CyPhyML::ValueFlowTarget &vfTarget, CyPhyML::unit& unitRef, std::string valueStr)
{
	if (vfTarget.type() == CyPhyML::Property::meta)
	{
		CyPhyML::Property ne = CyPhyML::Property::Cast(vfTarget);
		ne.Value() = valueStr;
		ne.ref() = unitRef;
	}
	else if (vfTarget.type() == CyPhyML::Parameter::meta)
	{
		CyPhyML::Parameter ne = CyPhyML::Parameter::Cast(vfTarget);
		ne.Value() = valueStr;
		ne.ref() = unitRef;
	}
	else if (vfTarget.type() == CyPhyML::Metric::meta)
	{
		CyPhyML::Metric ne = CyPhyML::Metric::Cast(vfTarget);
		ne.Value() = valueStr;
		ne.ref() = unitRef;
	}
	else
	{
		string message = "FormulaEvaluator -Only values of Property, Parameter, Metric, CADParameter, or CyberParameter can be set [" + vfTarget.getPath2("/", false) + "]";
		GMEConsole::Console::writeLine(message, MSG_ERROR);
		throw udm_exception (message);
	}
}

void NewTraverser::EvaluateLeafNodes(set<CyPhyML::ValueFlowTarget> &leafNodes)
{
	for (set<CyPhyML::ValueFlowTarget>::iterator i = leafNodes.begin(); i != leafNodes.end(); i++)
	{
		CyPhyML::ValueFlowTarget vfTarget (*i);
		//this->EvaluateValueFlowTarget(vfTarget);

		std::string name = vfTarget.name(), id = UdmGme::UdmId2GmeId(vfTarget.uniqueId());
#ifdef NO_UNIT_CHECK
		double result;
		//	DY 11/22/11: Checks the type so that ValueFormula leaf nodes don't accidentally get sent to EvaluatePPC()
		if (IsDerivedFrom(vfTarget.type(), CyPhyML::NamedElement::meta))
			this->EvaluatePPC(vfTarget, result);
		else if (IsDerivedFrom(vfTarget.type(), CyPhyML::ValueFormula::meta))
			this->EvaluateFormula(CyPhyML::ValueFormula::Cast(vfTarget), result);
#else 
		UnitUtil::ValueUnitRep uvRep;
			//	DY 11/22/11: Checks the type so that ValueFormula leaf nodes don't accidentally get sent to EvaluatePPC()
		if (   IsDerivedFrom(vfTarget.type(), CyPhyML::HasDescriptionAndGUID::meta)
			|| IsDerivedFrom(vfTarget.type(), CyPhyML::Constant::meta))
		{
			this->EvaluatePPC(vfTarget, uvRep);
		}
		else if (IsDerivedFrom(vfTarget.type(), CyPhyML::ValueFormula::meta))
			this->EvaluateFormula(CyPhyML::ValueFormula::Cast(vfTarget), uvRep);
#endif	
	}
}


// Another version that uses ValueUnitRep for unit conversion
bool NewTraverser::EvaluatePPC(CyPhyML::ValueFlowTarget &vf, UnitUtil::ValueUnitRep &myVURep)
{
	long id = vf.uniqueId();
	std::string name = vf.name(), idStr = UdmGme::UdmId2GmeId(vf.uniqueId());

	bool stat = 1;
	bool isNamedElement = 1;

	//  :::already evaluated :::
	map<long, UnitUtil::ValueUnitRep>::const_iterator ci = m_convertedValueFlowTargets_SIMap.find(id);
	if (ci != m_convertedValueFlowTargets_SIMap.end())
	{
		myVURep = ci->second;
		return 1;
	}
	
	std::string val;
	CyPhyML::ParamPropTarget unitRef;						// get vf's unitReference	
	CyPhyML::unit myUnit;
	Uml::Class vfType = vf.type();
	if (vfType == CyPhyML::Parameter::meta)
	{
		CyPhyML::Parameter tmp = CyPhyML::Parameter::Cast(vf);
		val = tmp.Value();
		unitRef = tmp.ref();
	}
	else if (vfType == CyPhyML::Property::meta)
	{
		CyPhyML::Property tmp = CyPhyML::Property::Cast(vf);
		val = tmp.Value();
		unitRef = tmp.ref();
	}
	else if (vfType == CyPhyML::Metric::meta)
	{
		CyPhyML::Metric tmp = CyPhyML::Metric::Cast(vf);
		val = tmp.Value();
		unitRef = tmp.ref();
	}
	else if (vfType == CyPhyML::Constant::meta)
	{
		CyPhyML::Constant tmp = CyPhyML::Constant::Cast(vf);

		// convert double to string
		std::ostringstream strs;
		strs << tmp.ConstantValue();
		val = strs.str();
	}
	else if (vfType == CyPhyML::ValueFlowTypeSpecification::meta)
	{
		CyPhyML::ValueFlowTypeSpecification tmp = CyPhyML::ValueFlowTypeSpecification::Cast(vf);
		val = "0";
		unitRef = tmp.ref();
		isNamedElement = 0;
	}
	else
	{
		GMEConsole::Console::writeLine(name + " [" + idStr + "] is not Parameter, CADParameter, CyberParameter, Metrics or Property! Skipping!", MSG_INFO);
	}

	//if (unitRef == Udm::null)		// null unit reference, let's throw an exception for now
	//	throw udm_exception(idStr + "'s unit reference is null!");

	bool nullUnitRef = (unitRef == Udm::null);

	string unitRefID = UdmGme::UdmId2GmeId(unitRef.uniqueId());	
	if (!nullUnitRef && IsDerivedFrom(unitRef.type(), CyPhyML::unit::meta))		// TODO: 12/20/11 Auto-assigning unit	//if (IsDerivedFrom(unitRef.type(), CyPhyML::unit::meta))					// only CyPhyML::unit is supported right now
		myUnit = CyPhyML::unit::Cast(unitRef);
	myVURep.cyphyRef = myUnit;													// TODO: 12/20/11 Auto-assigning unit

	// ::: not evaluated yet :::	
	set<CyPhyML::ValueFlow> VF_Set = RemoveVFsWithDstRefports(vf.srcValueFlow());
	if (VF_Set.size() > 0)
	{
		//double tmp = 0;
		UnitUtil::ValueUnitRep incomingVURep;
		unitUtil.ConvertToSIEquivalent(myUnit, 1, myVURep.unitRep);											

		if (VF_Set.size() > 1)
		{
			string message = "FormulaEvaluator -" + vf.getPath2("/", false) + " has >1 incoming ValueFlow connections.";
			GMEConsole::Console::writeLine(message, MSG_ERROR);
			throw udm_exception(message);
		}

		CyPhyML::ValueFlowTarget src_vfTarget = (CyPhyML::ValueFlow(*VF_Set.begin())).srcValueFlow_end();
		
		if (   IsDerivedFrom(src_vfTarget.type(), CyPhyML::HasDescriptionAndGUID::meta)
			|| IsDerivedFrom(src_vfTarget.type(), CyPhyML::Constant::meta))
		{
			EvaluatePPC(src_vfTarget, incomingVURep);

			if (incomingVURep.type == UnitUtil::ValueUnitRep::STRING)
			{
				if (myVURep.cyphyRef != Udm::null)
				{
					stat = 0;
					UpdateNamedElementValue(vf, 0);
					string message = "FormulaEvaluator -Unit incompatibility between " + vf.getPath2("/", false) + " and its incoming value flow."; 
					GMEConsole::Console::writeLine(message, MSG_ERROR);
					throw udm_exception(message);
				}
				myVURep.type = incomingVURep.type;
				myVURep.actualValue = incomingVURep.actualValue;
				myVURep.siValue = incomingVURep.siValue;
				myVURep.strValue = incomingVURep.strValue;
				UpdateNamedElementValue(vf, CyPhyML::unit::Cast(Udm::null), myVURep.strValue);
			}
			else if (myVURep.unitRep == incomingVURep.unitRep)				// units are compatible
			{
				// convert
				myVURep.type = UnitUtil::ValueUnitRep::DOUBLE;
				myVURep.siValue = incomingVURep.siValue;
				myVURep.actualValue = unitUtil.ConvertFromSIEquivalent(myUnit, incomingVURep.siValue);
				UpdateNamedElementValue(vf, myVURep.actualValue);
			}
			else														// units not compatible
			{
				myVURep.type = UnitUtil::ValueUnitRep::DOUBLE;
				if (nullUnitRef)		// TODO: 12/20/11 Auto-assigning unit
				{
					myVURep.siValue = incomingVURep.siValue;
					myVURep.cyphyRef = incomingVURep.cyphyRef;
					myVURep.unitRep = incomingVURep.unitRep;
					myVURep.actualValue = unitUtil.ConvertFromSIEquivalent(myVURep.cyphyRef, myVURep.siValue);
					UpdateNamedElementValue(vf, myVURep.cyphyRef, myVURep.actualValue);		// TODO: 12/20/11 Auto-assigning unit
					//GMEConsole::Console::writeLine("Unit Assignment: NamedElement [" + GMEConsole::Formatter::MakeObjectHyperlink(UdmGme::UdmId2GmeId(vf.uniqueId()), vf) + "]", MSG_WARNING);
				}
				else						// really are not compatible	
				{
					stat = 0;
					UpdateNamedElementValue(vf, 0);
					string message = "FormulaEvaluator -Unit incompatibility between " + vf.getPath2("/", false) + " and its incoming value flow."; 
					GMEConsole::Console::writeLine(message, MSG_ERROR);
					throw 
						udm_exception(message);
				}
			}
			
		}
		else if (IsDerivedFrom(src_vfTarget.type(), CyPhyML::ValueFormula::meta))
		{
			EvaluateFormula(CyPhyML::ValueFormula::Cast(src_vfTarget), incomingVURep);
			myVURep.type = UnitUtil::ValueUnitRep::DOUBLE;

			if (src_vfTarget.type() == CyPhyML::SimpleFormula::meta)			// simple formula
			{
				// check dimensionRep of myVURep and incomingVURep
				// assign siEqValue to siEqValue, then convert siEqValue to actualValue

				if (myVURep.unitRep == incomingVURep.unitRep)
				{
					myVURep.siValue = incomingVURep.siValue;
					myVURep.actualValue = unitUtil.ConvertFromSIEquivalent(myUnit, incomingVURep.siValue);
					UpdateNamedElementValue(vf, myVURep.actualValue);			// TODO: 12/20/11 Auto-assigning unit
				}
				else
				{
					if (nullUnitRef)		// TODO: 12/20/11 Auto-assigning unit
					{
						if (incomingVURep.cyphyRef == Udm::null)
						{
							string message = "FormulaEvaluator - Unit can not be deduced because unit reference is null [" + vf.getPath2("/", false) + "]";
							GMEConsole::Console::writeLine(message , MSG_ERROR);
							throw
								udm_exception(message);
						}	

						myVURep.siValue = incomingVURep.siValue;
						myVURep.cyphyRef = incomingVURep.cyphyRef;	
						myVURep.unitRep = incomingVURep.unitRep;
						myVURep.actualValue = unitUtil.ConvertFromSIEquivalent(myVURep.cyphyRef, myVURep.siValue); 						
						UpdateNamedElementValue(vf, myVURep.cyphyRef, myVURep.actualValue);		// TODO: 12/20/11 Auto-assigning unit
						//GMEConsole::Console::writeLine("Unit Assignment: NamedElement [" + GMEConsole::Formatter::MakeObjectHyperlink(UdmGme::UdmId2GmeId(vf.uniqueId()), vf) + "]", MSG_WARNING);			
					}
					else							// really are not compatible
					{
						string message = "FormulaEvaluator - Unit incompatibility between " + vf.getPath2("/", false) + " and its incoming value flow.";
						GMEConsole::Console::writeLine(message, MSG_ERROR);
						throw 
							udm_exception(message);
					}
				}
			}
			else																// custom formula
			{
				// no checking for dimensionRep of myVURep and incomingVURep
				// assign actualValue to actualValue, then convert actualValue to siEqValue 
				myVURep.actualValue = incomingVURep.actualValue;
				myVURep.siValue = unitUtil.ConvertToSIEquivalent(myUnit, incomingVURep.siValue, incomingVURep.unitRep);
				UpdateNamedElementValue(vf, myVURep.actualValue);
			}
			//UpdateNamedElementValue(vf, myVURep.actualValue);				// TODO: 12/20/11 Auto-assigning unit
		}

		m_convertedValueFlowTargets_SIMap[vf.uniqueId()] = myVURep;
		m_allNamedElements2Process.erase(vf);					//NamedElements2Process(vf);
	}
	else	
	{	
		/* standalone ValueFlowTarget
				use the value attribute on vftarget
				call ConvertToSIExponents() to convert value in terms of SI and get SI exponents (SIRep) populated	
		*/

		bool valueIsString;
		double tmp = std::numeric_limits<double>::quiet_NaN();
		if (val == "")
		{
			valueIsString = true;
		}
		else
		{
			// It may be a fraction or other expression.
			// We'll let MuParser handle it.
			mu::Parser parser;
			parser.ResetLocale();

			parser.SetExpr(CheckExpression(wstringFromUTF8(val)));

			try
			{
				tmp = parser.Eval();
				valueIsString = false;
			} 
			catch (mu::Parser::exception_type exc)
			{
				// Couldn't parse expression; assume it is a string
				valueIsString = true;
			}
		}

		if (valueIsString)
		{
			myVURep.type = UnitUtil::ValueUnitRep::STRING;
			myVURep.strValue = val;
			myVURep.actualValue = 0;			
			myVURep.siValue = 0;
			
			// This call will set myVURep.unitRep
			// We don't care what it returns, since the "siValue" of this node (a string) is 0.
			unitUtil.ConvertToSIEquivalent(myUnit, 0, myVURep.unitRep);
		}
		else
		{
			myVURep.type = UnitUtil::ValueUnitRep::DOUBLE;
			myVURep.siValue = myVURep.actualValue = tmp;	// unconverted value
			myVURep.siValue = unitUtil.ConvertToSIEquivalent(myUnit, tmp, myVURep.unitRep);		// converted value in terms of SI
		}
		m_convertedValueFlowTargets_SIMap[vf.uniqueId()] = myVURep;
		m_allNamedElements2Process.erase(vf);						//NamedElements2Process(vf);
	}

	return stat;
}


// Another version that takes units into account
bool NewTraverser::EvaluateFormula(CyPhyML::ValueFormula &vf, UnitUtil::ValueUnitRep &myVURep)
{	
	long id = vf.uniqueId();
	std::string name = vf.name(), idStr = UdmGme::UdmId2GmeId(vf.uniqueId());
	bool simpleFormula = (vf.type() == CyPhyML::SimpleFormula::meta);

	//  :::already evaluated :::
	map<long, UnitUtil::ValueUnitRep>::const_iterator ci = m_convertedValueFlowTargets_SIMap.find(vf.uniqueId());
	if (ci != m_convertedValueFlowTargets_SIMap.end())       // already evaluated before
	{
		myVURep = ci->second;
		return 1;
	}

	set<CyPhyML::ValueFlow> VF_Set = vf.srcValueFlow();
	if (VF_Set.size() == 0)
	{
		string message = "FormulaEvaluator - Formula has 0 inputs [" + vf.getPath2("/", false) + "]";
		GMEConsole::Console::writeLine(message, MSG_ERROR);
		throw udm_exception(message);
	}

	if (simpleFormula)			// simple formula
	{
		//vector<UnitUtil::ValueUnitRep> inputVURep_Collection;

		map<CyPhyML::ValueFlowTarget, UnitUtil::ValueUnitRep> inputVURep_Collection;
		multimap<wstring, double> parameters;

		// do conversion to SI units, check compatibility, then calculate and save		
		for (set<CyPhyML::ValueFlow>::iterator i = VF_Set.begin(); i != VF_Set.end(); i++)
		{
			UnitUtil::ValueUnitRep inputVURep;

			CyPhyML::ValueFlow src_vf(*i);
			CyPhyML::ValueFlowTarget src_vfTarget = src_vf.srcValueFlow_end();
			std::string vf_variableName = src_vf.FormulaVariableName();
			Uml::Class src_vfType = src_vfTarget.type();;
			if (IsDerivedFrom(src_vfType, CyPhyML::ValueFormula::meta))
			{				
				if (src_vfType == CyPhyML::CustomFormula::meta)
				{
					string message = "FormualEvaluator: SimpleFormula can only connect to CustomFormula through ValueFlowTypeSpecification objects [" + vf.getPath2("/", false) + "]";
					GMEConsole::Console::writeLine(message, MSG_ERROR);
					throw 
						udm_exception (message);
				}

				EvaluateFormula(CyPhyML::ValueFormula::Cast(src_vfTarget), inputVURep);
				//inputVURep_Collection.push_back(inputVURep);
				inputVURep_Collection[src_vfTarget] = inputVURep;
			
			}
			else
			{
				EvaluatePPC(src_vfTarget, inputVURep);
				if (inputVURep.type == UnitUtil::ValueUnitRep::STRING && inputVURep.strValue != "")
				{
					string message = "FormualEvaluator: SimpleFormula must not have string input [" + vf.getPath2("/", false) + "]";
					GMEConsole::Console::writeLine(message, MSG_ERROR);
					throw udm_exception (message);
				}
				//inputVURep_Collection.push_back(inputVURep);
				inputVURep_Collection[src_vfTarget] = inputVURep;
			}
		}
		
		int size = inputVURep_Collection.size();
		string method = CyPhyML::SimpleFormula::Cast(vf).Method();
		if (method != "Multiplication")		// need to check SIReps															
		{
			// if parameters size is greater than 1 then check unit compatibility, otherwise there's no point in checking

			if (size > 0)
			{
				map<CyPhyML::ValueFlowTarget, UnitUtil::ValueUnitRep>::const_iterator begin = inputVURep_Collection.begin();
				CyPhyML::ValueFlowTarget firstVFT = begin->first;
				UnitUtil::ValueUnitRep first = begin->second;
				parameters.insert(make_pair(L"dummy", first.siValue));
				begin++;

				for (; begin != inputVURep_Collection.end(); begin++)
				{
					if (first.unitRep != begin->second.unitRep)
					{
						string message = "FormulaEvaluator - SimpleFormula Input unit(s) NOT Compatible for multiplication[" + firstVFT.getPath2("/", false) + " ," + begin->first.getPath2("/", false) + "]";
						GMEConsole::Console::writeLine(message, MSG_ERROR);
						throw udm_exception(message);
					}
					else
						parameters.insert(make_pair(L"dummy", begin->second.siValue));
				}	


				// set DimensionRep
				myVURep.unitRep = first.unitRep;
				myVURep.cyphyRef = first.cyphyRef;	// TODO: 12/20/11 Auto-assigning unit

#if 0			
				parameters.insert(make_pair("dummy", inputVURep_Collection[0].siValue));


				for (unsigned int i = 1; i < size; i++)
				{
					if (inputVURep_Collection[0].unitRep != inputVURep_Collection[i].unitRep)
					{
						throw udm_exception("Input unit(s) NOT Compatible: Can't evaluate Formula [" + GMEConsole::Formatter::MakeObjectHyperlink(vf.name(), vf) + "]");
					}
					else
						parameters.insert(make_pair("dummy", inputVURep_Collection[i].siValue));
				}	


				// set DimensionRep
				myVURep.unitRep = inputVURep_Collection[0].unitRep;
				myVURep.cyphyRef = inputVURep_Collection[0].cyphyRef;	// TODO: 12/20/11 Auto-assigning unit
#endif
			}
		}
		else
		{
			// set DimensionRep
			for (map<CyPhyML::ValueFlowTarget, UnitUtil::ValueUnitRep>::const_iterator begin = inputVURep_Collection.begin(); begin != inputVURep_Collection.end(); begin++)
			{
				myVURep.unitRep += begin->second.unitRep;
				parameters.insert(make_pair(L"dummy", begin->second.siValue));
			}

#if 0
			// set DimensionRep
			for (unsigned int i = 0; i < size; i++)
			{
				myVURep.unitRep += inputVURep_Collection[i].unitRep;
				parameters.insert(make_pair("dummy", inputVURep_Collection[i].siValue));
			}
#endif
		}

		myVURep.siValue = EvaluateSimpleFormula(parameters, wstringFromUTF8(method));
		myVURep.actualValue = myVURep.siValue;
	}
	else					// custom formula treated as blackbox
	{
		std::multimap<std::wstring, double> parameters;

		for (set<CyPhyML::ValueFlow>::iterator i = VF_Set.begin(); i != VF_Set.end(); i++)
		{
			double tmp = 0;

			CyPhyML::ValueFlow src_vf(*i);
			CyPhyML::ValueFlowTarget src_vfTarget = src_vf.srcValueFlow_end();
			std::string vf_variableName = src_vf.FormulaVariableName();
			Uml::Class src_vfType = src_vfTarget.type();

			UnitUtil::ValueUnitRep incomingVURep;

			if (IsDerivedFrom(src_vfType, CyPhyML::ValueFormula::meta))              // formula connected to formula should not be valid anymore (???)
			{
				if (src_vfType == CyPhyML::SimpleFormula::meta)
				{
					string message = "FormulaEvaluator - CustomFormula can only connect to SimpleFormula using ValueFlowTypeSpecification objects [" + vf.getPath2("/", false) + "]";
					GMEConsole::Console::writeLine(message, MSG_ERROR);
					throw
						udm_exception (message);
				}

				//if (this->EvaluateFormula(CyPhyML::ValueFormula::Cast(src_vfTarget), incomingVURep)
				this->EvaluateFormula(CyPhyML::ValueFormula::Cast(src_vfTarget), incomingVURep);
				{
					parameters.insert(pair<std::wstring, double>((vf_variableName == "") ? L"Formula" : wstringFromUTF8(vf_variableName), incomingVURep.actualValue));
				}
			}				
			else 
			{
				//if (this->EvaluatePPC(src_vfTarget, incomingVURep))				
				this->EvaluatePPC(src_vfTarget, incomingVURep);
				if (incomingVURep.type == UnitUtil::ValueUnitRep::STRING && incomingVURep.strValue != "")
				{
					string message = "FormualEvaluator: SimpleFormula must not have string input [" + vf.getPath2("/", false) + "]";
					GMEConsole::Console::writeLine(message, MSG_ERROR);
					throw udm_exception (message);
				}
				{
                    parameters.insert(pair<std::wstring, double>((vf_variableName == "") ? wstringFromUTF8(src_vfTarget.name()) : wstringFromUTF8(vf_variableName), incomingVURep.actualValue));
				}
			}
	
		}
		
		if (parameters.size() > 0)
		{
            auto evaluatedValue = EvaluateCustomFormula(parameters, wstringFromUTF8(CyPhyML::CustomFormula::Cast(vf).Expression()));

			if (evaluatedValue == std::numeric_limits<double>::infinity())
			{
				string message = "FormulaEvaluator - Formula resulted in a value of +/- infinity. Skipping value update for [" + vf.getPath2("/", false) + "].";
				throw
					udm_exception (message);
			}
			else
			{
				myVURep.actualValue = evaluatedValue;
				myVURep.siValue = myVURep.actualValue;
			}
		}
		else
			return 0;
	}

	return 1;
}


void NewTraverser::PrintNodes(set<CyPhyML::ValueFlowTarget>& leafNodes, string type)
{
	GMEConsole::Console::writeLine("DEBUG: PrintNodes of type: " + type, MSG_INFO);
	for (set<CyPhyML::ValueFlowTarget>::const_iterator ci = leafNodes.begin(); ci != leafNodes.end(); ci++)
	{
		GMEConsole::Console::writeLine(GMEConsole::Formatter::MakeObjectHyperlink(UdmGme::UdmId2GmeId(ci->uniqueId()), *ci), MSG_INFO);
	}
}
void NewTraverser::EvaluateCarParameters()
{
	for (set<CyPhyML::CarParameter>::const_iterator ci = m_carParameters.begin(); ci != m_carParameters.end(); ci++)
	{
		set<CyPhyML::CarParameterPortMap> portMap_Set = ci->srcCarParameterPortMap();

		if (!portMap_Set.empty())
		{
			UnitUtil::ValueUnitRep myVURep;
			UnitUtil::ValueUnitRep incomingVURep;

			CyPhyML::ParamPropTarget unitRef = ci->ref();
			
			bool nullUnitRef = (unitRef == Udm::null);

			if (!nullUnitRef && IsDerivedFrom(unitRef.type(), CyPhyML::unit::meta))		// TODO: 12/20/11 Auto-assigning unit	//if (IsDerivedFrom(unitRef.type(), CyPhyML::unit::meta))
			{
				// only CyPhyML::unit is supported right now
				myVURep.cyphyRef = CyPhyML::unit::Cast(unitRef);
				unitUtil.ConvertToSIEquivalent(CyPhyML::unit::Cast(unitRef), 1, myVURep.unitRep);
			}


			if (portMap_Set.size() > 1)
			{
				string message = "FormulaEvaluator - CarParameter has >1 incoming portMap connection [" + ci->getPath2("/", false) + "]";
				GMEConsole::Console::writeLine(message, MSG_ERROR);
				throw udm_exception(message);
			}

			double value = 0;
			CyPhyML::ValueFlowTarget vft = portMap_Set.begin()->srcCarParameterPortMap_end();
			map<long, UnitUtil::ValueUnitRep>::iterator di = m_convertedValueFlowTargets_SIMap.find(vft.uniqueId());
			if (di != m_convertedValueFlowTargets_SIMap.end())
			{
				incomingVURep = di->second;
			}
			else
			{
				if (   IsDerivedFrom(vft.type(), CyPhyML::HasDescriptionAndGUID::meta)
					|| IsDerivedFrom(vft.type(), CyPhyML::Constant::meta))
				{
					EvaluatePPC(vft, incomingVURep);
				}
				else
				{
					string message = "FormulaEvaluator: CarParameter can not be directly connected to a formula [" + ci->getPath2("/", false) + "]";
					GMEConsole::Console::writeLine(message, MSG_ERROR);
					throw udm_exception(message);
				}
			}
			
			if (nullUnitRef)
			{
				// The unit ref of the ModelicaParameter is null.
				// What we will do is find the unit of the source ValueFlowTarget,
				// and set the unit ref to that, and also use the "actual value" as a value to match.
				if (incomingVURep.cyphyRef != Udm::null)
				{
					if (CyPhyML::CarParameter::Cast(ci->Archetype()) != Udm::null)
						CyPhyML::CarParameter::Cast(ci->Archetype()).ref() = incomingVURep.cyphyRef;
					else
						ci->ref() = incomingVURep.cyphyRef;
				}

				string tmp;
				to_string(tmp, incomingVURep.actualValue);

				ci->Value() = NonRealValueFixture(vft,tmp);
			}
			else
			{
				if (incomingVURep.cyphyRef == Udm::null)
				{
					string message ="FormulaEvaluator - CarParameter that references a unit must be connected to a value flow target that references a unit [" + ci->getPath2("/", false) + "]";
					GMEConsole::Console::writeLine(message, MSG_ERROR);
					throw udm_exception(message);
				}

				if (myVURep.unitRep == incomingVURep.unitRep)
				{
					string tmp;
					to_string(tmp, unitUtil.ConvertFromSIEquivalent(myVURep.cyphyRef, incomingVURep.siValue));
					
					ci->Value() = NonRealValueFixture(vft,tmp);					
				}
				else
				{
					string message = "FormulaEvaluator - CarParameter's unit is incompatible with incoming value flow target's unit [" + ci->getPath2("/", false) + "]";
					GMEConsole::Console::writeLine(message, MSG_ERROR);
					throw udm_exception(message);
				}
			}
		}
	}
}


// 7-3-2012: CADParameter inside CADModel instead of Component
void NewTraverser::EvaluateCADParameters()
{
	for (set<CyPhyML::CADParameter>::const_iterator ci = m_cadParameters.begin(); ci != m_cadParameters.end(); ci++)
	{
		set<CyPhyML::CADParameterPortMap> portMap_Set = ci->srcCADParameterPortMap();
		if (!portMap_Set.empty())
		{
			if (portMap_Set.size() > 1)
			{
				string message = "FormulaEvaluator - CADParameter has >1 incoming portMap connection [" + ci->getPath2("/", false) + "]";
				GMEConsole::Console::writeLine(message, MSG_ERROR);
				throw udm_exception(message);
			}

			CyPhyML::ValueFlowTarget vft = portMap_Set.begin()->srcCADParameterPortMap_end();
			map<long, UnitUtil::ValueUnitRep>::iterator di = m_convertedValueFlowTargets_SIMap.find(vft.uniqueId());

			std::string sourceName = vft.name();

			UnitUtil::ValueUnitRep incomingVURep;

			if (di != m_convertedValueFlowTargets_SIMap.end())
			{
				incomingVURep = di->second;
			}
			else		// incoming vft did not have another vft (vft --> cad parameter)
			{
				if (IsDerivedFrom(vft.type(), CyPhyML::ValueFormula::meta))
				{
					EvaluateFormula(CyPhyML::ValueFormula::Cast(vft), incomingVURep);
					double tmpval = incomingVURep.siValue;
				}
				else if (   IsDerivedFrom(vft.type(), CyPhyML::HasDescriptionAndGUID::meta)
					    || IsDerivedFrom(vft.type(), CyPhyML::Constant::meta))
				{
					EvaluatePPC(vft, incomingVURep);
				}
			}

			std::string tmp;
			string unit_name = ci->Unit();
			CyPhyML::unit cyphy_unit;
			if (unit_name == "")
			{
				// Try to get unit from reference
				if (ci->getReferencedObject() != Udm::null)
					cyphy_unit = CyPhyML::unit::Cast(ci->getReferencedObject());

			}
			if (cyphy_unit == Udm::null && unit_name != "")
			{
				cyphy_unit = FindUnitByName(unit_name);
				if (cyphy_unit == Udm::null)
				{
					string message = "FormulaEvaluator - Can not find a unit with name (" + unit_name + ") to do unit conversion for CADParameter [" + ci->getPath2("/", false) + "]";
					GMEConsole::Console::writeLine(message, MSG_ERROR);
					throw udm_exception (message);
				}
		
			}
			
			if (cyphy_unit != Udm::null)
				to_string(tmp, unitUtil.ConvertFromSIEquivalent(cyphy_unit, incomingVURep.siValue));
			else
				to_string (tmp, incomingVURep.actualValue);
			ci->Value() = NonRealValueFixture(vft,tmp);
		}
	}
}

void NewTraverser::EvaluateManufactureParameters()
{
	for (set<CyPhyML::ManufacturingModelParameter>::const_iterator ci = m_manufactureParameters.begin(); ci != m_manufactureParameters.end(); ci++)
	{
		set<CyPhyML::ManufacturingParameterPortMap> portMap_Set = ci->srcManufacturingParameterPortMap();
		if (!portMap_Set.empty())
		{
			UnitUtil::ValueUnitRep myVURep, incomingVURep;
			CyPhyML::ParamPropTarget unitRef = ci->ref();
			
			bool nullUnitRef = (unitRef == Udm::null);

			if (!nullUnitRef && IsDerivedFrom(unitRef.type(), CyPhyML::unit::meta))		// TODO: 12/20/11 Auto-assigning unit	//if (IsDerivedFrom(unitRef.type(), CyPhyML::unit::meta))
			{
				// only CyPhyML::unit is supported right now
				myVURep.cyphyRef = CyPhyML::unit::Cast(unitRef);
				unitUtil.ConvertToSIEquivalent(CyPhyML::unit::Cast(unitRef), 1, myVURep.unitRep);
			}

			if (portMap_Set.size() > 1)
			{
				string message = "FormulaEvaluator - ManufactureParameter has >1 incoming portMap connection [" + ci->getPath2("/", false) + "]";
				GMEConsole::Console::writeLine(message, MSG_ERROR);
				throw udm_exception(message);
			}

			double value = 0;
			CyPhyML::ValueFlowTarget vft = portMap_Set.begin()->srcManufacturingParameterPortMap_end();
			map<long, UnitUtil::ValueUnitRep>::iterator di = m_convertedValueFlowTargets_SIMap.find(vft.uniqueId());
			if (di != m_convertedValueFlowTargets_SIMap.end())
			{
				incomingVURep = di->second;
			}
			else
			{
				if (   IsDerivedFrom(vft.type(), CyPhyML::HasDescriptionAndGUID::meta)
					|| IsDerivedFrom(vft.type(), CyPhyML::Constant::meta))
				{
					EvaluatePPC(vft, incomingVURep);
				}
				else
				{
					string message = "FormualEvaluator: ManufactureParameter can not be directly connected to a formula [" + ci->getPath2("/", false) + "]";
					GMEConsole::Console::writeLine(message, MSG_ERROR);
					throw udm_exception(message);
				}
			}
			
			if (nullUnitRef)
			{
				string tmp;
				to_string(tmp, incomingVURep.siValue);
				ci->Value() = tmp;			
				if (incomingVURep.cyphyRef != Udm::null)
				{
					CyPhyML::ManufacturingModelParameter(ci->Archetype()).ref() = incomingVURep.cyphyRef; // ZL: change unit in the base type in order to support instances and subtypes
				}
			}
			else
			{
				if (incomingVURep.cyphyRef == Udm::null)
				{
					string message ="FormulaEvaluator - ManufactureParameter that references a unit must be connected to a value flow target that references a unit [" + ci->getPath2("/", false) + "]";
					GMEConsole::Console::writeLine(message, MSG_ERROR);
					throw udm_exception(message);
				}

				if (myVURep.unitRep == incomingVURep.unitRep)
				{
					string tmp;
					to_string(tmp, unitUtil.ConvertFromSIEquivalent(myVURep.cyphyRef, incomingVURep.siValue));
					ci->Value() = tmp;
					ci->Value() = NonRealValueFixture(vft,tmp);
				}
				else
				{
					string message = "FormulaEvaluator - ManufactureParameter's unit is incompatible with incoming value flow target's unit [" + ci->getPath2("/", false) + "]";
					GMEConsole::Console::writeLine(message, MSG_ERROR);
					throw udm_exception(message);
				}
			}
		}
	}
}

// ZL 11/20/2013 support modelica parameters as value flow targets
void NewTraverser::EvaluateModelicaParameters()
{
	// TODO: Implement this method
	
	for (set<CyPhyML::ModelicaParameter>::const_iterator ci = m_modelicaParameters.begin(); ci != m_modelicaParameters.end(); ci++)
	{
		set<CyPhyML::ModelicaParameterPortMap> portMap_Set = ci->srcModelicaParameterPortMap();

		if (!portMap_Set.empty())
		{
			UnitUtil::ValueUnitRep myVURep;
			UnitUtil::ValueUnitRep incomingVURep;

			CyPhyML::ParamPropTarget unitRef = ci->ref();
			
			bool nullUnitRef = (unitRef == Udm::null);

			if (!nullUnitRef && IsDerivedFrom(unitRef.type(), CyPhyML::unit::meta))		// TODO: 12/20/11 Auto-assigning unit	//if (IsDerivedFrom(unitRef.type(), CyPhyML::unit::meta))
			{
				// only CyPhyML::unit is supported right now
				myVURep.cyphyRef = CyPhyML::unit::Cast(unitRef);
				unitUtil.ConvertToSIEquivalent(CyPhyML::unit::Cast(unitRef), 1, myVURep.unitRep);
			}


			if (portMap_Set.size() > 1)
			{
				string message = "FormulaEvaluator - ModelicaParameter has >1 incoming portMap connection [" + ci->getPath2("/", false) + "]";
				GMEConsole::Console::writeLine(message, MSG_ERROR);
				throw udm_exception(message);
			}

			double value = 0;
			CyPhyML::ValueFlowTarget vft = portMap_Set.begin()->srcModelicaParameterPortMap_end();
			map<long, UnitUtil::ValueUnitRep>::iterator di = m_convertedValueFlowTargets_SIMap.find(vft.uniqueId());
			if (di != m_convertedValueFlowTargets_SIMap.end())
			{
				incomingVURep = di->second;
			}
			else
			{
				if (   IsDerivedFrom(vft.type(), CyPhyML::HasDescriptionAndGUID::meta)
					|| IsDerivedFrom(vft.type(), CyPhyML::Constant::meta))
				{
					EvaluatePPC(vft, incomingVURep);
				}
				else
				{
					string message = "FormulaEvaluator: ModelicaParameter can not be directly connected to a formula [" + ci->getPath2("/", false) + "]";
					GMEConsole::Console::writeLine(message, MSG_ERROR);
					throw udm_exception(message);
				}
			}
			
			if (nullUnitRef)
			{
				// The unit ref of the ModelicaParameter is null.
				// What we will do is find the unit of the source ValueFlowTarget,
				// and set the unit ref to that, and also use the "actual value" as a value to match.
				if (incomingVURep.cyphyRef != Udm::null)
				{
					if (CyPhyML::ModelicaParameter::Cast(ci->Archetype()) != Udm::null)
						CyPhyML::ModelicaParameter::Cast(ci->Archetype()).ref() = incomingVURep.cyphyRef;
					else
						ci->ref() = incomingVURep.cyphyRef;
				}

				string tmp;
				to_string(tmp, incomingVURep.actualValue);

				ci->Value() = NonRealValueFixture(vft,tmp);
			}
			else
			{
				if (incomingVURep.cyphyRef == Udm::null)
				{
					string message ="FormulaEvaluator - ModelicaParameter that references a unit must be connected to a value flow target that references a unit [" + ci->getPath2("/", false) + "]";
					GMEConsole::Console::writeLine(message, MSG_ERROR);
					throw udm_exception(message);
				}

				if (myVURep.unitRep == incomingVURep.unitRep)
				{
					string tmp;
					to_string(tmp, unitUtil.ConvertFromSIEquivalent(myVURep.cyphyRef, incomingVURep.siValue));
					
					ci->Value() = NonRealValueFixture(vft,tmp);					
				}
				else
				{
					string message = "FormulaEvaluator - ModelicaParameter's unit is incompatible with incoming value flow target's unit [" + ci->getPath2("/", false) + "]";
					GMEConsole::Console::writeLine(message, MSG_ERROR);
					throw udm_exception(message);
				}
			}
		}
	}
}


CyPhyML::unit NewTraverser::FindUnitByName(string unit_name_in)
{
	CyPhyML::unit cyphy_unit;
	map<string, CyPhyML::unit>::iterator i = m_unit_name_table.find(unit_name_in);
	if (i != m_unit_name_table.end())
	{
		cyphy_unit = i->second;
		//m_unit_name_table[unit_name_in] = cyphy_unit;
	}
	else
	{
		bool found = false;
		for (set<CyPhyML::Units>::const_iterator ci = m_units_folders.begin(); ci != m_units_folders.end(); ci++)
		{
			set<CyPhyML::unit> unit_set = ci->unit_kind_children();
			for (set<CyPhyML::unit>::const_iterator di = unit_set.begin(); di != unit_set.end(); di++)
			{
				if ((string)di->Symbol() == unit_name_in)
				{
					cyphy_unit = *di;
					m_unit_name_table[unit_name_in] = cyphy_unit;
					found = true;
					break;
				}
			}

			if (found)
				break;
		}
	}
	return cyphy_unit;
}

void NewTraverser::FindUnitsFolders(const Udm::Object &udm_obj_in)
{
	// [1] Find Top most RootFolder
	// [2] Traverse Top most RF to find all Units folder in the model

	CyPhyML::RootFolder top_rf = FindTopRootFolder(udm_obj_in);
	if (top_rf == Udm::null)
	{
		string message = "FormulaEvaluator - Can not find top level units folder, something is wrong [" + udm_obj_in.getPath2("/", false) + "]";
		GMEConsole::Console::writeLine(message, MSG_ERROR);
		throw udm_exception(message);
	}

	string top_rf_name = top_rf.name();
	FindUnitsFolders(top_rf);
	for (set<CyPhyML::Units>::const_iterator ci = m_units_folders.begin(); ci != m_units_folders.end(); ci++)
		string units_folder_name = ci->name();
}

void NewTraverser::FindUnitsFolders(const CyPhyML::RootFolder &root_folder_in)
{
	set<CyPhyML::TypeSpecifications> ts_set = root_folder_in.TypeSpecifications_kind_children();
	for (set<CyPhyML::TypeSpecifications>::const_iterator ci = ts_set.begin(); ci != ts_set.end(); ci++)
	{
		set<CyPhyML::Units> units_folder = ci->Units_kind_children();
		m_units_folders.insert(units_folder.begin(), units_folder.end());
	}

	set<CyPhyML::RootFolder> rf_set = root_folder_in.RootFolder_kind_children();
	for (set<CyPhyML::RootFolder>::const_iterator ci = rf_set.begin(); ci != rf_set.end(); ci++)
	{
		FindUnitsFolders(*ci);
	}
}

CyPhyML::RootFolder NewTraverser::FindTopRootFolder(const Udm::Object &udm_obj_in)
{
	CyPhyML::RootFolder top_rf;

	Udm::Object my_parent = udm_obj_in.GetParent();
	if ((string)my_parent.type().name() != "RootFolder")
		top_rf = FindTopRootFolder(my_parent);
	else
	{
		CyPhyML::RootFolder my_parent_rf = CyPhyML::RootFolder::Cast(my_parent);
		string my_parent_rf_name = my_parent_rf.name();		
		Udm::Object my_parent_parent = my_parent_rf.GetParent();
		if (my_parent_parent == Udm::null)
			top_rf = my_parent_rf;
		else
			top_rf = FindTopRootFolder(my_parent_parent);
	}

	return top_rf;
}


// 2-20-2013: Test Bench Suite
void NewTraverser::EvaluateTestBenchSuite(CyPhyML::TestBenchSuite &suite)
{
	string tmp;
	set<CyPhyML::ValueFlow> conn_set = suite.ValueFlow_kind_children();
	for (set<CyPhyML::ValueFlow>::const_iterator ci = conn_set.begin(); ci != conn_set.end(); ci++)
	{
		CyPhyML::ValueFlow vf_conn = *ci;
		CyPhyML::ValueFlowTarget src_vft = vf_conn.srcValueFlow_end();
		CyPhyML::ValueFlowTarget dst_vft = vf_conn.dstValueFlow_end();

		if (src_vft != Udm::null && dst_vft != Udm::null)
		{
			Uml::Class src_vftype = src_vft.type(), dst_vftype = dst_vft.type();
			if (!IsDerivedFrom(src_vftype, CyPhyML::HasDescriptionAndGUID::meta) || !IsDerivedFrom(dst_vftype, CyPhyML::HasDescriptionAndGUID::meta))
			{
				GMEConsole::Console::writeLine("Formulas are not supported for SOT value flow evaluation [SRC=>" + src_vft.getPath2("/", false) + ", DST=>" + dst_vft.getPath2("/", false) + "]", MSG_WARNING);
				continue;
			}

			UnitUtil::ValueUnitRep src_vurep, dst_vurep;
			if (GetVftUnitAndValue(src_vft, src_vurep) != "")
			{
				GetVftUnitAndValue(dst_vft, dst_vurep);

				if (src_vurep.unitRep == dst_vurep.unitRep)			// assign
				{
					to_string(tmp, src_vurep.siValue);
					dst_vurep.siValue = src_vurep.siValue;
					//GMEConsole::Console::writeLine((string)src_vft.name() + ": " + tmp, MSG_INFO);
					dst_vurep.actualValue = unitUtil.ConvertFromSIEquivalent(dst_vurep.cyphyRef, src_vurep.siValue);
					UpdateNamedElementValue(dst_vft, dst_vurep.actualValue);
				}
				else
				{
					if (dst_vurep.cyphyRef == Udm::null)			// auto-assign unit
					{
						dst_vurep.siValue = src_vurep.siValue;
						dst_vurep.cyphyRef = src_vurep.cyphyRef;
						dst_vurep.unitRep = src_vurep.unitRep;
						dst_vurep.actualValue = unitUtil.ConvertFromSIEquivalent(dst_vurep.cyphyRef, dst_vurep.siValue);
						UpdateNamedElementValue(dst_vft, dst_vurep.cyphyRef, dst_vurep.actualValue);		// TODO: 12/20/11 Auto-assigning unit
					}
					else											// really not compatible
					{
						string message = "FormulaEvaluator - Unit mismatch in SOT Valueflow [SRC=>" + src_vft.getPath2("/", false) + ", DST=>" + dst_vft.getPath2("/", false) + "]";
						GMEConsole::Console::writeLine(message, MSG_ERROR);
						throw udm_exception(message);
					}
				}
			}
		}
	}
}

string NewTraverser::GetVftUnitAndValue(const CyPhyML::ValueFlowTarget& vft, UnitUtil::ValueUnitRep& vurep)
{
	string tmp;
	string val = "";
	CyPhyML::ParamPropTarget unitRef;						// get vf's unitReference	
	CyPhyML::unit myUnit;
	Uml::Class vft_type = vft.type();

	if (vft_type == CyPhyML::Parameter::meta)
	{
		CyPhyML::Parameter tmp = CyPhyML::Parameter::Cast(vft);
		val = tmp.Value();
		unitRef = tmp.ref();
	}
	else if (vft_type == CyPhyML::Property::meta)
	{
		CyPhyML::Property tmp = CyPhyML::Property::Cast(vft);
		val = tmp.Value();
		unitRef = tmp.ref();
	}
	else if (vft_type == CyPhyML::Metric::meta)
	{
		CyPhyML::Metric tmp = CyPhyML::Metric::Cast(vft);
		val = tmp.Value();
		unitRef = tmp.ref();
		// Metrics that are connected to CADComputation/Mass are in Kilograms META-2756
		if (unitRef == Udm::null)
		{
			std::set<CyPhyML::CADComputation2Metric> srcMetrics = tmp.srcCADComputation2Metric();
			for (auto it = srcMetrics.begin(); it != srcMetrics.end(); it++)
			{
				if (Uml::IsDerivedFrom(static_cast<CyPhyML::CADComputationType>(it->srcCADComputation2Metric_end()).type(), CyPhyML::Mass::meta))
				{
					vurep.actualValue = vurep.siValue = std::atof(val.c_str());
					vurep.unitRep = UnitUtil::DimensionRep::zeroes;
					vurep.unitRep.mass = 1;
					vurep.cyphyRef = CyPhyML::unit::Cast(Udm::null);
					return val;
				}
			}
		}
	}
	else if (vft_type == CyPhyML::Constant::meta)
	{
		CyPhyML::Constant tmp = CyPhyML::Constant::Cast(vft);

		// convert double to string
		std::ostringstream strs;
		strs << tmp.ConstantValue();
		val = strs.str();
	}
	else if (vft_type == CyPhyML::ValueFlowTypeSpecification::meta)
	{
		CyPhyML::ValueFlowTypeSpecification tmp = CyPhyML::ValueFlowTypeSpecification::Cast(vft);
		val = "0";
		unitRef = tmp.ref();
	}

	if (unitRef != Udm::null)
	{
		if (IsDerivedFrom(unitRef.type(), CyPhyML::unit::meta))
			myUnit = CyPhyML::unit::Cast(unitRef);
	}
	vurep.cyphyRef = myUnit;	

	if (val == "")
		unitUtil.ConvertToSIEquivalent(myUnit, 1, vurep.unitRep);
	else
		vurep.siValue = unitUtil.ConvertToSIEquivalent(myUnit, std::atof(val.c_str()), vurep.unitRep);

	return val;
}

std::string NewTraverser::NonRealValueFixture( CyPhyML::ValueFlowTarget &vft, std::string &value )
{
	std::string rtn = value;

	// Test for non real value fixture
	std::string strValue = "";

	if (vft.type() == CyPhyML::Property::meta)
	{
		auto prop = CyPhyML::Property::Cast(vft);
		strValue = prop.Value();
	}
	else if (vft.type() == CyPhyML::Parameter::meta)
	{
		auto parameter = CyPhyML::Parameter::Cast(vft);
		strValue = parameter.Value();
	}
	else if (vft.type() == CyPhyML::Constant::meta)
	{
		auto tmp = CyPhyML::Constant::Cast(vft);

		// convert double to string
		std::ostringstream strs;
		strs << tmp.ConstantValue();
		strValue = strs.str();
	}

	if (strValue != "")
	{
		double dValue = std::atof(strValue.c_str());
		if (dValue == 0)
		{
			// could be matrix or vector
			string numberAsString = "";
			to_string(numberAsString, dValue);
			if (strcmp(numberAsString.c_str(), strValue.c_str()) == 0)
			{
				// strings are equal, i.e. the 0 value was parsed correctly
			}
			else
			{
				// it is not a double therefore leave it as it is.
				 rtn = strValue;
			}
		}
	}

	return rtn;
}


std::wstring wstringFromUTF8(const std::string& utf8)
{
    if (utf8.empty())
        return std::wstring();

    // Fail if an invalid input character is encountered
    const DWORD conversionFlags = MB_ERR_INVALID_CHARS;

    const int utf16Length = ::MultiByteToWideChar(CP_UTF8, conversionFlags, utf8.data(), utf8.length(), NULL, 0);
    if (utf16Length == 0)
    {
        DWORD error = ::GetLastError();

        throw udm_exception(
            (error == ERROR_NO_UNICODE_TRANSLATION) ?
            "Invalid UTF-8 sequence found in input string." :
            "Can't get length of UTF-16 string (MultiByteToWideChar failed).");
    }

    std::unique_ptr<wchar_t[]> utf16(new wchar_t[utf16Length]);
    if (utf16 == NULL)
        throw std::bad_alloc();

    if (!::MultiByteToWideChar(CP_UTF8, 0, utf8.data(), utf8.length(), utf16.get(), utf16Length))
    {
        DWORD error = ::GetLastError();
        throw udm_exception("Can't convert string from UTF-8 to UTF-16 (MultiByteToWideChar failed).");
    }

    return std::wstring(utf16.get(), utf16.get() + utf16Length);
}

std::wstring wstringFromUTF8(const Udm::StringAttr& attr) {
    return wstringFromUTF8(static_cast<const std::string>(attr));
}
