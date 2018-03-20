#include "cc_ApplyModelConstraints.h"
#include "cc_CADFactoryAbstract.h"


namespace isis
{
	
bool ApplyListedModelsConstraints( 
			const std::string									&in_AssemblyComponentID,
			const std::list<std::string>							&in_ComponentIDsToBeConstrained, 
			bool													in_AllowUnconstrainedModels,
			std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,
																// Provide for the case where the first assembled part does not have
																// the datums front, top, and right defined. 
			bool													in_FirstComponentToBePositionedAsIntiiallyPlaced_IfDatumsCannotBeFound )  
																							throw (isis::application_exception)
{


	isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
	isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();
	isis::cad::IModelOperations&         modelOperations = cAD_Factory_ptr->getModelOperations();

	bool fail = false;
	
	try
	{
		int count = 0;
		for ( std::list<std::string>::const_iterator i(in_ComponentIDsToBeConstrained.begin()); 
			  i != in_ComponentIDsToBeConstrained.end(); ++i, ++count )
		{

			//std::cout << std::endl << "*** in_AllowUnconstrainedModels: " << in_AllowUnconstrainedModels;
			//std::cout << std::endl << "*** Number of Constraints:       " << in_CADComponentData_map[*i].constraintDef.constraints.size();

			if ( in_AllowUnconstrainedModels && in_CADComponentData_map[*i].constraintDef.constraints.size() == 0 )
			{
				isis_LOG(lg, isis_FILE, isis_INFO) << "      "  << 	in_CADComponentData_map[*i].name << "::Unconstrained";
				continue;
			}
			if ( in_CADComponentData_map[*i].constraintDef.constraints.size() == 0 )
			{
				std::stringstream errorString;
				errorString <<
					"Unconstrained Component, Component Instance ID: " <<  in_CADComponentData_map[*i].componentID << "  Component Name: " << in_CADComponentData_map[*i].name;
				throw isis::application_exception("C04002",errorString.str().c_str());			
			}
			
			try
			{

				//bool stop = Apply_CADDatum_ModelConstraints(	in_Factory, 
				//									*in_assembly_model,
				//									*i,
				//									in_CADComponentData_map[*i].constraintDef, 
				//									in_CADComponentData_map,
				//									in_IgnoreGuides);	
				//bool stop = Apply_CADDatum_ModelConstraints_2(//	in_Factory, 
				//									//*in_assembly_model,
				//									in_AssemblyComponentID,
				//									*i,
				//									in_CADComponentData_map[*i].constraintDef, 
				//									in_CADComponentData_map );		

				bool stop = modelOperations.applySingleModelConstraints(		in_AssemblyComponentID,
																			*i,
																			in_CADComponentData_map );	


				if (stop)
				{
					fail = true;
					break;
				}
			}
			catch (...)
			{
				if ( count == 0 && in_FirstComponentToBePositionedAsIntiiallyPlaced_IfDatumsCannotBeFound)
				{
					isis_LOG(lg, isis_FILE, isis_INFO) << "      "  << 	in_CADComponentData_map[*i].name << "::Coordinate System --> Assembly Coordinate System";
				}
				else
				{
					throw;
				}
			}  // END try - Catch
			isis_LOG(lg, isis_FILE, isis_INFO) << "      "  << 	in_CADComponentData_map[*i].name << "::Coordinate System --> Assembly Coordinate System";
		}
	}
	catch (...)
	{

		throw;
	}
	return fail;
} // End ApplyListedModelsConstraints



///////////////////////////////////////////////////////////////////////////////////////////////////////
bool ComponentIDChildOf(		const std::string								&in_ChildID,
							const std::string								&in_ParentID,
							std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map )
{
	bool found = false;

	for (	std::list<std::string>::const_iterator i( in_CADComponentData_map[in_ParentID].children.begin() ); 
			i != in_CADComponentData_map[in_ParentID].children.end(); ++i )
	{
		//clog << std::endl << "   ****** in_ComponentID, i->ComponentID(): " << in_ComponentID << "  " << i->ComponentID();
		if ( *i == in_ChildID )
		{
			//clog << std::endl << "   return true;";
			return true;

		}
		else
		{
			found = ComponentIDChildOf( in_ChildID,   *i , in_CADComponentData_map);
		}

		if (found)
		{
			//clog << std::endl << "   return true;";
			return true;
		}

	}
	//clog << std::endl << "   return found:  "  << found;
	return found;
}



}