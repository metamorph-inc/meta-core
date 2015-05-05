#include "CreoModelMetaData.h"
#include "CreoMetaData.h"
#include <sstream>

CreoModelMetaData::CreoModelMetaData(std::string filename)
{
	Udm::SmartDataNetwork ai(CreoMetaData::diagram);
	try{
		ai.OpenExisting(filename,"CreoMetaData", Udm::CHANGES_PERSIST_ALWAYS);
	} catch (...)
	{
		// Can't open file, don't do anything
		return;
	}

	CreoMetaData::MetaData MetaData_ptr = CreoMetaData::MetaData::Cast(ai.GetRootObject());		

	vector<CreoMetaData::ConstraintData> constraints = MetaData_ptr.ConstraintData_kind_children();

	for (vector<CreoMetaData::ConstraintData>::iterator cd = constraints.begin(); cd != constraints.end(); ++cd)
	{
		ConstraintData d;
		d.attr = cd->Flags();
		d.compid1 = cd->InstanceGUID1();
		d.compid2 = cd->InstanceGUID2();
		d.dname1 = cd->DatumName1();
		d.dname2 = cd->DatumName2();
		data.push_back(d);
	}

	vector<CreoMetaData::InitialPosition> initpos = MetaData_ptr.InitialPosition_kind_children();

	for (vector<CreoMetaData::InitialPosition>::iterator cd = initpos.begin(); cd != initpos.end(); ++cd)
	{
		double* m = new double[16];
		istringstream str(cd->matrix());
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				str >> m[i*4+j];
			}
		}
		initialpos[cd->GUID()] = m;
	}
}

const CreoModelMetaData::ConstraintData* CreoModelMetaData::Match(std::string iguid1, std::string iguid2, std::string dname1, std::string dname2)
{
	for (vector<ConstraintData>::iterator cd = data.begin(); cd != data.end(); ++cd)
	{
		if (cd->compid1!=iguid1) continue;
		if (cd->compid2!=iguid2) continue;
		if (cd->dname1!=dname1) continue;
		if (cd->dname2!=dname2) continue;
		return cd._Ptr;
	}
	return 0;
}

const double* CreoModelMetaData::GetInitialPos(std::string guid)
{
	if (initialpos.find(guid) != initialpos.end())
	{
		return initialpos[guid];
	} else {
		return 0;
	}
}