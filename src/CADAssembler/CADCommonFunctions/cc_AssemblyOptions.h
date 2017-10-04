#ifndef CC_ASSEMBLY_OPTIONS_H
#define CC_ASSEMBLY_OPTIONS_H

#include <string>
using namespace std;

namespace isis
{

class AssemblyOptions
{
	static AssemblyOptions Instance;
public:
	enum eStopAssembly
	{
		DONTSTOP,
		AFTER_GUIDE,
		AFTER_CONSTRAINTS,
	};
	enum eFailLevel
	{
		DONTFAIL,
		FEATURE_REDEF,
		REGEN,
	};
	std::string StopAssemblyComponentID; // Debug feature, stop the assembly when this component has been added
	eStopAssembly StopAssemblyAt; // Stop the assembly when?
	eFailLevel FailLevel; // Fail at which level?
	bool FullRegen; // Do a full regen between components constrained
	bool ApplyParamsAfter; // Apply Cyphy parameters after assembly has been completed
	static AssemblyOptions& Create(const string &input);
	static const AssemblyOptions& GetInstance()
	{
		return Instance;
	}
private:
	AssemblyOptions() : StopAssemblyComponentID(""), StopAssemblyAt(DONTSTOP), FailLevel(DONTFAIL), FullRegen(false), ApplyParamsAfter(false)
	{}
}; // class AssemblyOptions

} // namespace isis

#endif