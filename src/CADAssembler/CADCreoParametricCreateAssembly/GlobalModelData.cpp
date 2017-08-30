#include "stdafx.h"
#include "GlobalModelData.h"
#include "ProObjects.h"

namespace isis{

	GlobalModelData& GlobalModelData::Instance = GlobalModelData();

	void GlobalModelData::Clear()
	{
		CadComponentData.clear();
		CadAssemblies.DataExchangeSpecifications.clear();
		CadAssemblies.materials.clear();
		CadAssemblies.topLevelAssemblies.clear();
		CadAssemblies.unassembledComponents.clear();
		ComponentEdit.avmId.clear();
		ComponentEdit.cADModel_ptr = 0;
		ComponentEdit.resourceId.clear();
	}

	void GlobalModelData::Lock()
	{
		locker.lock();
	}

	void GlobalModelData::UnLock()
	{
		locker.unlock();
	}

	ProSolid GlobalModelData::GetTopModel()
	{
		return static_cast<ProSolid>(CadComponentData[CadAssemblies.topLevelAssemblies.begin()->assemblyComponentID].cADModel_hdl);
	}

	ProSolid GlobalModelData::GetModelFromGuid(const std::string &guid)
	{
		for (std::map<std::string, isis::CADComponentData>::const_iterator it = CadComponentData.begin(); it != CadComponentData.end(); ++it)
		{
			if (it->second.componentID==guid) return static_cast<ProSolid>(it->second.cADModel_hdl);
		}
		return 0;
	}

	/**
	This reverse key for cad component data has one parts.
	  1) the ProSolid (ProMdl) identifier

	  ** This appoach may result in an incorrect guid **
	*/
	std::string GlobalModelData::GetGuidFromModel(ProSolid sld)
	{
		for (std::map<std::string, isis::CADComponentData>::const_iterator it = CadComponentData.begin(); it != CadComponentData.end(); ++it)
		{
			isis::CADComponentData candidate = it->second;
			if (candidate.cADModel_hdl != sld) continue;
			return candidate.componentID;
		}
		return "";
	}

	/**
	The reverse key for cad component data has three parts.
	  1) the ProSolid (ProMdl) identifier
	  2) the ProAsmcomppath : where in the assembly the component is placed
	  3) the assembly tree : ascending the tree should lead to the top
	*/
    inline bool areEquivalentCollectionsOfIntegers(const std::list<int> &lhs, const ProIdTable &rhs) {
		if (lhs.size() != sizeof(rhs)) return false;
		int ix=0;
		std::list<int>::const_iterator lhsIter;
        for (lhsIter = lhs.begin(); lhsIter != lhs.end(); ++lhsIter, ++ix) {
			if (*lhsIter != rhs[ix]) return false;
		}
		return true;
	}
	/**
	Is the lhs a prefix of the rhs?
	*/
	inline bool isPrefixMatch(const std::list<int> &lhs, const ProIdTable &rhs) {
		if (lhs.size() > sizeof(rhs)) return false;
		int ix=0;
		std::list<int>::const_iterator lhsIter;
        for (lhsIter = lhs.begin(); lhsIter != lhs.end(); ++lhsIter, ++ix) {
			if (*lhsIter != rhs[ix]) return false;
		}
		return true;
	}
	inline bool isInTopComponent(const std::string in_Top, std::map<std::string, isis::CADComponentData> &in_CompMap, const isis::CADComponentData &in_Candidate) {
		CADComponentData ancestor = in_Candidate;
		// and empty parent component id indicates the genesis of the tree
		while (! ancestor.parentComponentID.empty()) {
		    ancestor = in_CompMap[ancestor.parentComponentID];
		}
		return (ancestor.componentID == in_Top) ? true : false;
	}
	std::string GlobalModelData::GetGuidFromModel(const ProAsmcomppath &in_CompPath, const ProSolid &sld)
	{
		std::string top = CadAssemblies.topLevelAssemblies.begin()->assemblyComponentID;
		for (std::map<std::string, isis::CADComponentData>::const_iterator it = CadComponentData.begin(); it != CadComponentData.end(); ++it)
		{
			isis::CADComponentData candidate = it->second;
			// if (candidate.modelHandle != sld) continue;
			std::list<int> asmPath = it->second.componentPaths;
			// if ( ! areEquivalentCollectionsOfIntegers(candidate.componentPaths, in_CompPath.comp_id_table) ) continue;
			if ( ! isPrefixMatch( candidate.componentPaths, in_CompPath.comp_id_table ) ) continue;
			if ( ! isInTopComponent(top, CadComponentData, candidate) ) continue;
			return candidate.componentID;
		}
		return "";
	}
	
	std::string GlobalModelData::GetAvmIdFromModel(ProSolid sld)
	{
		for (std::map<std::string, isis::CADComponentData>::const_iterator it = CadComponentData.begin(); it != CadComponentData.end(); ++it)
		{
			if (it->second.cADModel_hdl==sld) return it->second.avmComponentId;
		}
		return "";
	}
}