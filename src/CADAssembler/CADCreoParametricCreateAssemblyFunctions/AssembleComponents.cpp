#include <AssembleComponents.h>

namespace isis
{



ProError AddComponentToAssembly( ProAssembly asm_model, 
							     ProSolid	 comp_model,
							     ProAsmcomp	 *out_p_assembled_feat_handle,
								 ProMatrix   matrix) 
											throw (isis::application_exception)
{
	ProError status;

	ProAsmcomp asmcomp;

	//-----------------------------------------------------------------
	//	Package the component initially
	//-----------------------------------------------------------------
	isis::isis_ProAsmcompAssemble (asm_model, comp_model, matrix, &asmcomp);
	//ProTKPrintf(" \n ProAsmcompAssemble Error Code:  %d", status);

	*out_p_assembled_feat_handle = asmcomp;			

	return PRO_TK_NO_ERROR;
}


} // end namespace isis