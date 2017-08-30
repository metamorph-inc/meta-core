#include "stdafx.h"
#include "ProObjects.h"

namespace isis
{

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
    mode = UNDEFINEDMODE;
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
    for(std::map<std::string, isis::CADComponentData>::const_iterator it = CadComponentData.begin(); it != CadComponentData.end(); ++it)
    {
        if(it->second.componentID==guid)
        {
            return static_cast<ProSolid>(it->second.cADModel_hdl);
        }
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
    for(std::map<std::string, isis::CADComponentData>::const_iterator it = CadComponentData.begin(); it != CadComponentData.end(); ++it)
    {
        isis::CADComponentData candidate = it->second;
        if(candidate.cADModel_hdl != sld)
        {
            continue;
        }
        return candidate.componentID;
    }
    return "";
}

/**
The reverse key for cad component data has three parts.
  1) the ProSolid (ProMdl) identifier
  2) the ProAsmcomppath : where in the assembly the component is placed
  3) the assembly tree : ascending the tree should lead to the top
  The ProSolid should match but does not provide a unique identifier; the ProSolid
  may appear at multiple places in the assembly tree.
*/
inline bool areEquivalentCollectionsOfIntegers(const std::list<int> &lhs, const ProIdTable &rhs)
{
    if(lhs.size() != sizeof(rhs))
    {
        return false;
    }
    int ix=0;
    std::list<int>::const_iterator lhsIter;
    for(lhsIter = lhs.begin(); lhsIter != lhs.end(); ++lhsIter, ++ix)
    {
        if(*lhsIter != rhs[ix])
        {
            return false;
        }
    }
    return true;
}
/**
Is the lhs a prefix of the rhs?
- a prefix must be no longer than its subject
- a matching length of 0 indicates not a matching prefix
*/
inline int isPrefixMatch(const std::list<int> &lhs, const ProIdTable &rhs)
{
    if(lhs.size() > sizeof(rhs))
    {
        return 0;
    }
    int ix=0;
    std::list<int>::const_iterator lhsIter;
    for(lhsIter = lhs.begin(); lhsIter != lhs.end(); ++lhsIter, ++ix)
    {
        if(*lhsIter != rhs[ix])
        {
            return 0;
        }
    }
    return ix;
}
inline bool isInTopComponent(const std::string in_Top, std::map<std::string, isis::CADComponentData> &in_CompMap, const isis::CADComponentData &in_Candidate)
{
    CADComponentData ancestor = in_Candidate;
    // and empty parent component id indicates the genesis of the tree
    while(! ancestor.parentComponentID.empty())
    {
        ancestor = in_CompMap[ancestor.parentComponentID];
    }
    return (ancestor.componentID == in_Top) ? true : false;
}
/**
This method searches for the component which is lowest in the assembly tree.
*/
std::string GlobalModelData::GetGuidFromModel(const ProAsmcomppath &in_CompPath)
{
    const std::string top = CadAssemblies.topLevelAssemblies.begin()->assemblyComponentID;
    isis::CADComponentData best_candidate;
    int best_length = 0;
    for(std::map<std::string, isis::CADComponentData>::const_iterator it = CadComponentData.begin(); it != CadComponentData.end(); ++it)
    {
        isis::CADComponentData candidate = it->second;
        if(! isInTopComponent(top, CadComponentData, candidate))
        {
            continue;
        }

        std::list<int> asmPath = it->second.componentPaths;
        int prefix_length = isPrefixMatch(candidate.componentPaths, in_CompPath.comp_id_table);
        // isis::GlobalModelData::Instance.m_isis_LOG(lg, isis_FILE, isis_WARN)
        //	<< " prefix: " << prefix_length << isis_EOL
        //	<< "candidate: " << candidate.componentID;
        if(prefix_length <= best_length)
        {
            continue;
        }
        best_length = prefix_length;
        best_candidate = candidate;
    }
    return best_candidate.componentID;
}

const isis::CADComponentData *GlobalModelData::GetComponentDataFromModel(const ProAsmcomppath &in_CompPath)
{
    std::string top = CadAssemblies.topLevelAssemblies.begin()->assemblyComponentID;
    int max = -10000;
    const isis::CADComponentData *maxval = 0;
    for(std::map<std::string, isis::CADComponentData>::const_iterator it = CadComponentData.begin(); it != CadComponentData.end(); ++it)
    {
        isis::CADComponentData candidate = it->second;
        //if (candidate.modelHandle != sld) continue;
        std::list<int> asmPath = it->second.componentPaths;
        // if ( ! areEquivalentCollectionsOfIntegers(candidate.componentPaths, in_CompPath.comp_id_table) ) continue;
        int curr = isPrefixMatch(candidate.componentPaths, in_CompPath.comp_id_table);
        if(curr > max)
        {
            max = curr;
            maxval = &(it->second);
        }
//			if ( ! isInTopComponent(top, CadComponentData, candidate) ) continue;

        //return &(it->second);
    }
    if(maxval)
    {
        return maxval;
    }
    else
    {
        return 0;
    }
}

}