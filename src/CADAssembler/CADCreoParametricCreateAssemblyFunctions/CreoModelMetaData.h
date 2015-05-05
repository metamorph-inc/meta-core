#ifndef _MODELCONSTRAINTDATA_
#define _MODELCONSTRAINTDATA_

#include <string>
#include <vector>
#include <map>

class CreoModelMetaData
{
public:
	struct ConstraintData
	{
		std::string compid1;
		std::string compid2;
		std::string dname1;
		std::string dname2;
		int attr;
	};
	CreoModelMetaData(std::string filename);
	const CreoModelMetaData::ConstraintData* Match(std::string iguid1, std::string iguid2, std::string dname1, std::string dname2);
	const double* GetInitialPos(std::string guid);
private:
	std::vector<ConstraintData> data;
	std::map<std::string, double*> initialpos;
};



#endif