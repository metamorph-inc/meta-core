#include <ProEStructuresUtils.h>

namespace isis
{

//////////////////////////////////////////////////////////////////////////////////////////////////////
void Add_dependsOn (  std::map<std::string, CADComponentData> &in_out_CADComponentData_map )
{
	for ( std::map<std::string, CADComponentData>::iterator i(in_out_CADComponentData_map.begin());
		  i != in_out_CADComponentData_map.end();
		  ++i )
	{
		for ( std::vector<ConstraintData>::const_iterator j( i->second.constraintDef.constraints.begin());
			  j != i->second.constraintDef.constraints.end();
			  ++j )
		{
			for (	std::vector<ConstraintPair>::const_iterator k(j->constraintPairs.begin());
					k != j->constraintPairs.end();
					++k )
			{
				for (	std::vector<ConstraintFeature>::const_iterator l(k->constraintFeatures.begin());
						l != k->constraintFeatures.end();
						++l )
				{
					if (  i->first != l->componentInstanceID  ) i->second.dependsOn.insert( l->componentInstanceID );
				}
			}
		}
	}	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

} // END namespace isis