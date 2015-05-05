#include <CreoErrorCodes.h>

namespace isis
{
	const std::string ProToolKitError_string(  ProErrors in_Error)
	{	

		std::string tempstring;

		switch (in_Error)
		{
		   case PRO_TK_NO_ERROR: tempstring = "PRO_TK_NO_ERROR"; break;		
		   case PRO_TK_GENERAL_ERROR: tempstring = "PRO_TK_GENERAL_ERROR"; break;		
		   case PRO_TK_BAD_INPUTS: tempstring = "PRO_TK_BAD_INPUTS"; break;		
		   case PRO_TK_USER_ABORT: tempstring = "PRO_TK_USER_ABORT"; break;		
		   case PRO_TK_E_NOT_FOUND: tempstring = "PRO_TK_E_NOT_FOUND"; break;		
		   case PRO_TK_E_FOUND: tempstring = "PRO_TK_E_FOUND"; break;		
		   case PRO_TK_LINE_TOO_LONG: tempstring = "PRO_TK_LINE_TOO_LONG"; break;		
		   case PRO_TK_CONTINUE: tempstring = "PRO_TK_CONTINUE"; break;		
		   case PRO_TK_BAD_CONTEXT: tempstring = "PRO_TK_BAD_CONTEXT"; break;		
		   case PRO_TK_NOT_IMPLEMENTED: tempstring = "PRO_TK_NOT_IMPLEMENTED"; break;	
		   case PRO_TK_OUT_OF_MEMORY: tempstring = "PRO_TK_OUT_OF_MEMORY"; break;		
		   case PRO_TK_COMM_ERROR: tempstring = "PRO_TK_COMM_ERROR"; break;		
		   case PRO_TK_NO_CHANGE: tempstring = "PRO_TK_NO_CHANGE"; break;		
		   case PRO_TK_SUPP_PARENTS: tempstring = "PRO_TK_SUPP_PARENTS"; break;		
		   case PRO_TK_PICK_ABOVE: tempstring = "PRO_TK_PICK_ABOVE"; break;		
		   case PRO_TK_INVALID_DIR: tempstring = "PRO_TK_INVALID_DIR"; break;		
		   case PRO_TK_INVALID_FILE: tempstring = "PRO_TK_INVALID_FILE"; break;		
		   case PRO_TK_CANT_WRITE: tempstring = "PRO_TK_CANT_WRITE"; break;		
		   case PRO_TK_INVALID_TYPE: tempstring = "PRO_TK_INVALID_TYPE"; break;		
		   case PRO_TK_INVALID_PTR: tempstring = "PRO_TK_INVALID_PTR"; break;		
		   case PRO_TK_UNAV_SEC: tempstring = "PRO_TK_UNAV_SEC"; break;		
		   case PRO_TK_INVALID_MATRIX: tempstring = "PRO_TK_INVALID_MATRIX"; break;		
		   case PRO_TK_INVALID_NAME: tempstring = "PRO_TK_INVALID_NAME"; break;		
		   case PRO_TK_NOT_EXIST: tempstring = "PRO_TK_NOT_EXIST"; break;		
		   case PRO_TK_CANT_OPEN: tempstring = "PRO_TK_CANT_OPEN"; break;		
		   case PRO_TK_ABORT: tempstring = "PRO_TK_ABORT"; break;			
		   case PRO_TK_NOT_VALID: tempstring = "PRO_TK_NOT_VALID"; break;		
		   case PRO_TK_INVALID_ITEM: tempstring = "PRO_TK_INVALID_ITEM"; break;		
		   case PRO_TK_MSG_NOT_FOUND: tempstring = "PRO_TK_MSG_NOT_FOUND"; break;		
		   case PRO_TK_MSG_NO_TRANS: tempstring = "PRO_TK_MSG_NO_TRANS"; break;		
		   case PRO_TK_MSG_FMT_ERROR: tempstring = "PRO_TK_MSG_FMT_ERROR"; break;		
		   case PRO_TK_MSG_USER_QUIT: tempstring = "PRO_TK_MSG_USER_QUIT"; break;		
		   case PRO_TK_MSG_TOO_LONG: tempstring = "PRO_TK_MSG_TOO_LONG"; break;		
		   case PRO_TK_CANT_ACCESS: tempstring = "PRO_TK_CANT_ACCESS"; break;		
		   case PRO_TK_OBSOLETE_FUNC: tempstring = "PRO_TK_OBSOLETE_FUNC"; break;		
		   case PRO_TK_NO_COORD_SYSTEM: tempstring = "PRO_TK_NO_COORD_SYSTEM"; break;	
		   case PRO_TK_E_AMBIGUOUS: tempstring = "PRO_TK_E_AMBIGUOUS"; break;		
		   case PRO_TK_E_DEADLOCK: tempstring = "PRO_TK_E_DEADLOCK"; break;		
		   case PRO_TK_E_BUSY: tempstring = "PRO_TK_E_BUSY"; break;			
		   case PRO_TK_E_IN_USE: tempstring = "PRO_TK_E_IN_USE"; break;		
		   case PRO_TK_NO_LICENSE: tempstring = "PRO_TK_NO_LICENSE"; break;		
		   case PRO_TK_BSPL_UNSUITABLE_DEGREE: tempstring = "PRO_TK_BSPL_UNSUITABLE_DEGREE"; break;
		   case PRO_TK_BSPL_NON_STD_END_KNOTS: tempstring = "PRO_TK_BSPL_NON_STD_END_KNOTS"; break;
		   case PRO_TK_BSPL_MULTI_INNER_KNOTS: tempstring = "PRO_TK_BSPL_MULTI_INNER_KNOTS"; break;
		   case PRO_TK_BAD_SRF_CRV: tempstring = "PRO_TK_BAD_SRF_CRV"; break;		
		   case PRO_TK_EMPTY: tempstring = "PRO_TK_EMPTY"; break;
		   case PRO_TK_BAD_DIM_ATTACH: tempstring = "PRO_TK_BAD_DIM_ATTACH"; break;
		   case PRO_TK_NOT_DISPLAYED: tempstring = "PRO_TK_NOT_DISPLAYED"; break;
		   case PRO_TK_CANT_MODIFY: tempstring = "PRO_TK_CANT_MODIFY"; break;
		   case PRO_TK_CHECKOUT_CONFLICT: tempstring = "PRO_TK_CHECKOUT_CONFLICT"; break;
		   case PRO_TK_CRE_VIEW_BAD_SHEET: tempstring = "PRO_TK_CRE_VIEW_BAD_SHEET"; break;
		   case PRO_TK_CRE_VIEW_BAD_MODEL: tempstring = "PRO_TK_CRE_VIEW_BAD_MODEL"; break;
		   case PRO_TK_CRE_VIEW_BAD_PARENT: tempstring = "PRO_TK_CRE_VIEW_BAD_PARENT"; break;
		   case PRO_TK_CRE_VIEW_BAD_TYPE: tempstring = "PRO_TK_CRE_VIEW_BAD_TYPE"; break;
		   case PRO_TK_CRE_VIEW_BAD_EXPLODE: tempstring = "PRO_TK_CRE_VIEW_BAD_EXPLODE"; break;
		   case PRO_TK_UNATTACHED_FEATS: tempstring = "PRO_TK_UNATTACHED_FEATS"; break;
		   case PRO_TK_REGEN_AGAIN: tempstring = "PRO_TK_REGEN_AGAIN"; break;
		   case PRO_TK_DWGCREATE_ERRORS: tempstring = "PRO_TK_DWGCREATE_ERRORS"; break;
		   case PRO_TK_UNSUPPORTED: tempstring = "PRO_TK_UNSUPPORTED"; break;
		   case PRO_TK_NO_PERMISSION: tempstring = "PRO_TK_NO_PERMISSION"; break;
		   case PRO_TK_AUTHENTICATION_FAILURE: tempstring = "PRO_TK_AUTHENTICATION_FAILURE"; break;
		   case PRO_TK_OUTDATED: tempstring = "PRO_TK_OUTDATED"; break;
		   case PRO_TK_INCOMPLETE: tempstring = "PRO_TK_INCOMPLETE"; break;
		   case PRO_TK_CHECK_OMITTED: tempstring = "PRO_TK_CHECK_OMITTED"; break;
		   case PRO_TK_MAX_LIMIT_REACHED: tempstring = "PRO_TK_MAX_LIMIT_REACHED"; break;
		   case PRO_TK_OUT_OF_RANGE: tempstring = "PRO_TK_OUT_OF_RANGE"; break;
		   case PRO_TK_APP_CREO_BARRED: tempstring = "PRO_TK_APP_CREO_BARRED"; break;	
		   case PRO_TK_APP_TOO_OLD: tempstring = "PRO_TK_APP_TOO_OLD"; break;
		   case PRO_TK_APP_BAD_DATAPATH: tempstring = "PRO_TK_APP_BAD_DATAPATH"; break;
		   case PRO_TK_APP_BAD_ENCODING: tempstring = "PRO_TK_APP_BAD_ENCODING"; break;
		   case PRO_TK_APP_NO_LICENSE: tempstring = "PRO_TK_APP_NO_LICENSE"; break;	
		   case PRO_TK_APP_XS_CALLBACKS: tempstring = "PRO_TK_APP_XS_CALLBACKS"; break;	
		   case PRO_TK_APP_STARTUP_FAIL: tempstring = "PRO_TK_APP_STARTUP_FAIL"; break;	
		   case PRO_TK_APP_INIT_FAIL: tempstring = "PRO_TK_APP_INIT_FAIL"; break;	
		   case PRO_TK_APP_VERSION_MISMATCH: tempstring = "PRO_TK_APP_VERSION_MISMATCH"; break;
		   case PRO_TK_APP_COMM_FAILURE: tempstring = "PRO_TK_APP_COMM_FAILURE"; break;	
		   case PRO_TK_APP_NEW_VERSION: tempstring = "PRO_TK_APP_NEW_VERSION"; break;	
		   case PRO_TK_APP_UNLOCK: tempstring = "PRO_TK_APP_UNLOCK"; break;	
		   default:
			   tempstring = "UNKNOWN";
		}

		return tempstring;	
	}


} // END namespace isis