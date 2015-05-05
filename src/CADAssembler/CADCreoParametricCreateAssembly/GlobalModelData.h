#ifndef GLOBALMODELDATA_H
#define GLOBALMODELDATA_H

#include <boost\thread\mutex.hpp>

namespace isis{

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
		ComponentEditInfo ComponentEdit; // Only in component edit mode
		 // Wipe out everything
		void Clear();
		void Lock();
		void UnLock();
		ProSolid GetTopModel();
		ProSolid GetModelFromGuid(const std::string &guid);

		std::string GetGuidFromModel(ProSolid sld);
		std::string GetGuidFromModel(const ProAsmcomppath &in_CompPath, const ProSolid &sld);
		std::string GetAvmIdFromModel(ProSolid sld);
	private:
		GlobalModelData()
		{
			mode = UNDEFINEDMODE;
		}
		boost::mutex locker;

	};

}


#endif