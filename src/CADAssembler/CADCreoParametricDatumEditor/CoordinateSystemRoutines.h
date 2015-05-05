#ifndef COORDINATE_SYSTEM_ROUTINES_H
#define COORDINATE_SYSTEM_ROUTINES_H

#include <isis_application_exception.h>
#include <isis_include_ptc_headers.h>
#include <string>
#include <vector>

namespace isis
{
	void CreateCoordinateSystem( ProMdl	in_p_model, 
								//ProMdlType in_pro_model_type,
								const ProCsys	   &referenceCoordinateSystem,
								const std::string &in_CoordinateSystemName,
								const std::vector<double> &in_Orgin,
								const std::vector<double> &in_XVector,
								const std::vector<double> &in_YVector ) throw(isis::application_exception);


	void FindDefaultCoordinateSystem( ProMdl in_p_model,
										  const std::string   &in_ModelName,
										  ProCsys &out_p_csys )
													throw(isis::application_exception);

	bool FoundProAxisBasedOnAxisName(	ProMdl				in_p_model,
										const std::string	&in_AxisName,	
										ProAxis				&out_p_axis)	throw(isis::application_exception);


} // END namespace isis

#endif