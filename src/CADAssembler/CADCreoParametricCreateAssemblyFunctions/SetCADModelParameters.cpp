#include <SetCADModelParameters.h>
#include <iostream>
#include <string>
#include <StringToEnumConversions.h>
#include <ParametricParameters.h>
#include "CommonDefinitions.h"
#include <map>
#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>


namespace isis
{

void ApplyParametricParameters( std::list<std::string>                          &in_ComponentIDs, 
							   std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
							   std::vector<isis::CADCreateAssemblyError>		&out_ErrorList)
																		throw (isis::application_exception)
{
	log4cpp::Category& logcat_fileonly = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);

	for ( std::list<std::string>::const_iterator t = in_ComponentIDs.begin(); t != in_ComponentIDs.end(); ++t )
	{
		// to avoid dereferencing
		CADComponentData *cadata = &in_CADComponentData_map[*t];

		std::string ModelNameWithSuffix = AmalgamateModelNameWithSuffix ( cadata->name, cadata->modelType );
		

		if ( cadata->parametricParametersPresent )
		{
			for( std::list<CADParameter>::const_iterator p( cadata->parametricParameters.begin());
			p != cadata->parametricParameters.end();
			++ p )
			{
				logcat_fileonly.infoStream() << "Set Component Parameter: ";
				logcat_fileonly.infoStream() << "   ModelNameWithSuffix: " << ModelNameWithSuffix;
				logcat_fileonly.infoStream() << "   in_CADComponentData_map[*t].p_model: " << cadata->p_model;
				logcat_fileonly.infoStream() << "    p->name:   " <<	p->name;
				logcat_fileonly.infoStream() << "    p->type:   " <<	CADParameterType_string(p->type);
				logcat_fileonly.infoStream() << "    p->value:  " <<	p->value;
				try 
				{
					SetParametricParameter( ModelNameWithSuffix, cadata->p_model, p->name, p->type, p->value);

				} catch (isis::application_exception &ex)
				{
					out_ErrorList.push_back(CADCreateAssemblyError(ex.what(), CADCreateAssemblyError_Severity_Warning));
				}
			}
		}

		// Regen 5 times. It may not work for the 1st time
		int i = 5;
		while (true)
		{
			try{
				i--;
				isis_ProSolidRegenerate((ProSolid)cadata->modelHandle, PRO_REGEN_NO_RESOLVE_MODE);
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
			isis::SetParametricParameter( FORCE_KEY, ModelNameWithSuffix, cadata->p_model, CYPHY_NAME, CAD_STRING, cadata->displayName);
		} catch (isis::application_exception &ex)
		{
			out_ErrorList.push_back(CADCreateAssemblyError(ex.what(), CADCreateAssemblyError_Severity_Warning));
		}
	}

}


} // end namespace isis