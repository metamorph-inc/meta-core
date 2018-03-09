#include <SetCADModelParameters.h>
#include <iostream>
#include <string>
#include <CreoStringToEnumConversions.h>
#include <ParametricParameters.h>
#include "cc_CommonDefinitions.h"
#include <map>
#include "cc_LoggerBoost.h"


namespace isis
{

void ApplyParametricParameters( std::list<std::string>                          &in_ComponentIDs, 
							   std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
							   std::vector<isis::CADCreateAssemblyError>		&out_ErrorList)
																		throw (isis::application_exception)
{
	

	for ( std::list<std::string>::const_iterator t = in_ComponentIDs.begin(); t != in_ComponentIDs.end(); ++t )
	{
		// to avoid dereferencing
		CADComponentData *cadata = &in_CADComponentData_map[*t];

		std::string ModelNameWithSuffix = AmalgamateModelNameWithSuffix ( cadata->name, cadata->modelType );
		

		if ( cadata->parametricParametersPresent )
		{
			ProMdl* p_model = (ProMdl*)cadata->cADModel_ptr_ptr;
			bool is_mmKs;
			ParametricParameter_WarnForPartUnitsMismatch(*cadata, &is_mmKs);

			for( std::list<CADParameter>::const_iterator p( cadata->parametricParameters.begin());
			p != cadata->parametricParameters.end();
			++ p )
			{
				isis_LOG(lg, isis_FILE, isis_INFO) << "Set Component Parameter: ";
				isis_LOG(lg, isis_FILE, isis_INFO) << "   ModelNameWithSuffix: " << ModelNameWithSuffix;
				isis_LOG(lg, isis_FILE, isis_INFO) << "   in_CADComponentData_map[*t].cADModel_ptr_ptr: " << (const void*)cadata->cADModel_ptr_ptr;
				isis_LOG(lg, isis_FILE, isis_INFO) << "    p->name:   " <<	p->name;
				isis_LOG(lg, isis_FILE, isis_INFO) << "    p->type:   " <<	CADParameterType_string(p->type);
				isis_LOG(lg, isis_FILE, isis_INFO) << "    p->value:  " <<	p->value;
				try 
				{
					SetParametricParameter( ModelNameWithSuffix, cadata->cADModel_ptr_ptr, p->name, p->type, p->value, p->units, is_mmKs);

				} catch (isis::application_exception &ex)
				{
					out_ErrorList.push_back(CADCreateAssemblyError(ex.what(), CADCreateAssemblyError_Severity_Warning));
				}
			}
			// R.O. 8/25/2015 Moved the following isis_ProSolidRegenerate to be within "if ( cadata->parametricParametersPresent )", previously 
			// this code was after "if ( cadata->parametricParametersPresent )" {...} and was being done for all parts. It is only
			// necessary for parts that have parameters.
			// Regen 5 times. It may not work for the 1st time
			int i = 5;
			while (true)
			{
				try{
					i--;
					isis_ProSolidRegenerate((ProSolid)cadata->cADModel_hdl, PRO_REGEN_NO_RESOLVE_MODE);
					break;
				} catch (isis::application_exception &ex)
				{
					if (i==0)
					{
						throw isis::application_exception("C06051", "Solid regeneration failed after applying parameters: " + ModelNameWithSuffix);
					}
				}
			}
	

			/*clog << std::endl << "Set Assembly Parameters: ";
			clog << std::endl << "   CYPHY_COMPONENT_INSTANCE_ID: " << cadata->componentID << std::endl;

			try {
				isis::SetParametricParameter( FORCE_KEY, ModelNameWithSuffix, cadata->p_model, CYPHY_COMPONENT_INSTANCE_ID, CAD_STRING, cadata->componentID);
			} catch (isis::application_exception &ex)
			{
				out_ErrorList.push_back(CADCreateAssemblyError(ex.what(), CADCreateAssemblyError_Severity_Warning));
			}
			*/
			try {
				if (cadata->displayName.size() >= MAX_STRING_PARAMETER_LENGTH )
				{
					cadata->displayName	= cadata->displayName.substr(0, MAX_STRING_PARAMETER_LENGTH-3)+"...";
				}
				isis::SetParametricParameter( FORCE_KEY, ModelNameWithSuffix, cadata->cADModel_ptr_ptr, CYPHY_NAME, CAD_STRING, cadata->displayName);
			} catch (isis::application_exception &ex)
			{
				out_ErrorList.push_back(CADCreateAssemblyError(ex.what(), CADCreateAssemblyError_Severity_Warning));
			}

		} // END if ( cadata->parametricParametersPresent )


	} // END for ( std::list<std::string>::const_iterator t = in_ComponentIDs.begin(); t != in_ComponentIDs.end(); ++t )

}

} // end namespace isis