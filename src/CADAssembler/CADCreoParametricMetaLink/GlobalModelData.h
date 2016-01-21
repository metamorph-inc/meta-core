#ifndef GLOBALMODELDATA_H
#define GLOBALMODELDATA_H

#include <boost/thread/mutex.hpp>
#include <MetaLinkHandler.h>

namespace isis
{

// This singleton class is intended to hold global information about the current model under edit via Meta-Link
class GlobalModelData
{
public:
    MetaLinkMode mode;
    static GlobalModelData& Instance;
    // Data about the model under edit
    std::map<std::string, isis::CADComponentData> CadComponentData;
    isis::CADAssemblies CadAssemblies;
    std::string designId;
	std::string instanceId;
    ComponentEditInfo ComponentEdit; // Only in component edit mode
    isis::MetaLinkHandler *metalink_handler_ptr;
    isis::MetaLinkAssemblyEditor::Pointer metalinkAssemblyEditorPtr;

    // Wipe out everything
    void Clear();
    void Lock();
    void UnLock();
    ProSolid GetTopModel();
    ProSolid GetModelFromGuid(const std::string &guid);

    std::string GetGuidFromModel(ProSolid sld);
    std::string GetGuidFromModel(const ProAsmcomppath &in_CompPath);
    const isis::CADComponentData *GetComponentDataFromModel(const ProAsmcomppath &in_CompPath);

    // log4cpp::Category& m_logcat;
private:
    GlobalModelData() : mode(UNDEFINEDMODE) //,
        //m_logcat(log4cpp::Category::getInstance(std::string("metalink.global.data")))
    {
    }
    boost::mutex locker;
};

}


#endif