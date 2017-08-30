#ifndef  MATERIAL_PROPERTIES_H
#define  MATERIAL_PROPERTIES_H

#include "isis_application_exception.h"
#include "cc_CommonStructures.h"

namespace isis
{

void PopulateAnalysisMaterialStruct( const Material				 &in_Material, 
									 AnalysisMaterialProperties  &out_AnalysisMaterialProperties_struct )
																		throw (isis::application_exception);

void stream_AnalysisMaterialProperties( const AnalysisMaterialProperties &in_CADAnalyses, std::ostream &out_Stream );

void ComputeAllowableStressLevels(	int										in_NumberOfCycles,
									const AnalysisMaterialProperties		&in_AnalysisMaterialProperties,
								    AnalysisMaterialProperties_Allowables	&out_AnalysisMaterialProperties )
																				throw (isis::application_exception);

void stream_AnalysisMaterialProperties_Allowables( 
									const AnalysisMaterialProperties_Allowables	&in_AnalysisMaterialProperties,
									std::ostream &out_Stream );		

void ReadMaterialsLibrary(  const std::string				&in_MaterialLibrary_PathAndFileName,
							const std::set<std::string>		&in_MaterialNames,
							std::map<std::string, Material> &out_Materials ) 
												 throw (isis::application_exception);

} // End namespace isis

#endif