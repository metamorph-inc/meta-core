#include "AssemblyOptions.h"
#include "boost\algorithm\string.hpp"
#include <boost\lexical_cast.hpp>
#include "cc_LoggerBoost.h"
#include <vector>

namespace isis{

AssemblyOptions AssemblyOptions::Instance;

// Format of input string: optionname=value[;optionname=value][;optionname=value]...
// Option names:
// - stopassembly=<ComponentID>:<0|1> stops assembling after the component added. 0 means stop after the guide, 1 means stop after the full constraint
// - faillevel=<0|1|2> 0 means fail if feature redefine fails, 1 means fail if regen failed, 2 means don't fail during assembly
// - fullregen=<0|1> 1 means full assembly regen after adding each part during the assembly
// - applyparamsafter=<0|1> 1 means apply the cyphy parameters after the assembly has been completed
AssemblyOptions& AssemblyOptions::Create(const string &input)
{
	
	vector<string> tokens;
	boost::split(tokens, input, boost::is_any_of(";"));
	for (vector<string>::const_iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		string keyword;
		string value;
		int eq = it->find("=");
		if (eq == string::npos)
		{
			keyword = it->substr(0);
		} else {
			keyword = it->substr(0, eq);
			value = it->substr(keyword.size()+1);
		}
		if (value.size()==0)
		{
			isis_LOG(lg, isis_FILE, isis_WARN) << "Invalid assembly option, no value specified. keyword=" << keyword;
			continue;
		}
		if (keyword=="applyparamsafter")
		{
			Instance.ApplyParamsAfter = value=="1";
		} else if (keyword=="stopassembly")
		{
			vector<string> tokens;
			boost::split(tokens, value, boost::is_any_of(":"));
			if (tokens.size()==0)
			{
				isis_LOG(lg, isis_FILE, isis_WARN) << "Invalid assembly option, wrong value for stopassembly. value=" << value;
				continue;
			}
			Instance.StopAssemblyComponentID = tokens[0];
			try{
				Instance.StopAssemblyAt = (eStopAssembly)(::boost::lexical_cast<int>(tokens[1])+1);
			} catch (...)
			{
				// conversion from string to int failed
				isis_LOG(lg, isis_FILE, isis_WARN) << "Invalid assembly option, wrong value for stopassembly. value=" << value;
			}
		} else if (keyword=="faillevel")
		{
			Instance.FailLevel = (eFailLevel)::boost::lexical_cast<int>(value);
		} else if (keyword=="fullregen")
		{
			Instance.FullRegen = value=="1";
		} else {
			isis_LOG(lg, isis_FILE, isis_WARN) << "Invalid assembly option, wrong keyword. keyword=" << keyword;
		}
	}
	return Instance;
}

}; // namespace isis