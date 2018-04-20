#ifndef ASSEMBLE_COMPONENTS_H
#define ASSEMBLE_COMPONENTS_H
#include <isis_ptc_toolkit_functions.h>
#include <isis_application_exception.h>
#include <isis_include_ptc_headers.h>
#include <iostream>
#include <ostream>

namespace isis
{


	ProError AddComponentToAssembly( ProAssembly		asm_model, 
								     ProSolid		comp_model,
								     ProAsmcomp		*out_p_assembled_feat_handle, 
									 ProMatrix		matrix) 
												throw (isis::application_exception);


}



#endif // end ASSEMBLE_COMPONENTS_H