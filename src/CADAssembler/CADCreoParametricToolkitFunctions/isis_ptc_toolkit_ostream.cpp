
#include "isis_ptc_toolkit_ostream.h"


namespace isis
{
	
	/**
	Should show the names of the status messages.
	Not just the numerical values.
	*/
	std::ostream& operator <<(std::ostream & out, const ProError & status) {
		switch ( status ) {
			case PRO_TK_NO_ERROR: 
			out << "PRO_TK_NO_ERROR"; 
			return out;
		case PRO_TK_GENERAL_ERROR: 
			out << "PRO_TK_GENERAL_ERROR"; 
			return out;
		}
		out << static_cast<int>(status);
		return out;
		/*
  PRO_TK_BAD_INPUTS		= -2,
  PRO_TK_USER_ABORT		= -3,
  PRO_TK_E_NOT_FOUND		= -4,
  PRO_TK_E_FOUND		= -5,
  PRO_TK_LINE_TOO_LONG		= -6,
  PRO_TK_CONTINUE		= -7,
  PRO_TK_BAD_CONTEXT		= -8,
  PRO_TK_NOT_IMPLEMENTED	= -9,
  PRO_TK_OUT_OF_MEMORY		= -10,
  PRO_TK_COMM_ERROR		= -11, // communication error 
  PRO_TK_NO_CHANGE		= -12,
  PRO_TK_SUPP_PARENTS		= -13,
  PRO_TK_PICK_ABOVE		= -14,
  PRO_TK_INVALID_DIR		= -15,
  PRO_TK_INVALID_FILE		= -16,
  PRO_TK_CANT_WRITE		= -17,
  PRO_TK_INVALID_TYPE		= -18,
  PRO_TK_INVALID_PTR		= -19,
  PRO_TK_UNAV_SEC		= -20,
  PRO_TK_INVALID_MATRIX		= -21,
  PRO_TK_INVALID_NAME		= -22,
  PRO_TK_NOT_EXIST		= -23,
  PRO_TK_CANT_OPEN		= -24,
  PRO_TK_ABORT			= -25,
  PRO_TK_NOT_VALID		= -26,
  PRO_TK_INVALID_ITEM		= -27,
  PRO_TK_MSG_NOT_FOUND		= -28,
  PRO_TK_MSG_NO_TRANS		= -29,
  PRO_TK_MSG_FMT_ERROR		= -30,
  PRO_TK_MSG_USER_QUIT		= -31,
  PRO_TK_MSG_TOO_LONG		= -32,
  PRO_TK_CANT_ACCESS		= -33,
  PRO_TK_OBSOLETE_FUNC		= -34,
  PRO_TK_NO_COORD_SYSTEM	= -35,
  PRO_TK_E_AMBIGUOUS		= -36,
  PRO_TK_E_DEADLOCK		= -37,
  PRO_TK_E_BUSY			= -38,
  PRO_TK_E_IN_USE		= -39,
  PRO_TK_NO_LICENSE		= -40,
  PRO_TK_BSPL_UNSUITABLE_DEGREE = -41,
  PRO_TK_BSPL_NON_STD_END_KNOTS = -42,
  PRO_TK_BSPL_MULTI_INNER_KNOTS = -43,
  PRO_TK_BAD_SRF_CRV		= -44,
  PRO_TK_EMPTY                  = -45,
  PRO_TK_BAD_DIM_ATTACH         = -46,
  PRO_TK_NOT_DISPLAYED          = -47,
  PRO_TK_CANT_MODIFY            = -48,
  PRO_TK_CHECKOUT_CONFLICT      = -49,
  PRO_TK_CRE_VIEW_BAD_SHEET     = -50,
  PRO_TK_CRE_VIEW_BAD_MODEL     = -51,
  PRO_TK_CRE_VIEW_BAD_PARENT    = -52,
  PRO_TK_CRE_VIEW_BAD_TYPE      = -53,
  PRO_TK_CRE_VIEW_BAD_EXPLODE   = -54,
  PRO_TK_UNATTACHED_FEATS       = -55,
  PRO_TK_REGEN_AGAIN            = -56,
  PRO_TK_DWGCREATE_ERRORS       = -57,
  PRO_TK_UNSUPPORTED            = -58,
  PRO_TK_NO_PERMISSION          = -59,
  PRO_TK_AUTHENTICATION_FAILURE = -60,
  PRO_TK_OUTDATED               = -61,
  PRO_TK_INCOMPLETE             = -62,
  PRO_TK_CHECK_OMITTED          = -63,
  PRO_TK_MAX_LIMIT_REACHED      = -64,
  PRO_TK_OUT_OF_RANGE           = -65, 
  PRO_TK_CHECK_LAST_ERROR	= -66,

  PRO_TK_APP_CREO_BARRED	= -88,
  PRO_TK_APP_TOO_OLD            = -89,
  PRO_TK_APP_BAD_DATAPATH       = -90,
  PRO_TK_APP_BAD_ENCODING       = -91,
  PRO_TK_APP_NO_LICENSE 	= -92,
  PRO_TK_APP_XS_CALLBACKS	= -93,
  PRO_TK_APP_STARTUP_FAIL	= -94,
  PRO_TK_APP_INIT_FAIL	        = -95,
  PRO_TK_APP_VERSION_MISMATCH   = -96,
  PRO_TK_APP_COMM_FAILURE	= -97,
  PRO_TK_APP_NEW_VERSION	= -98,
  PRO_TK_APP_UNLOCK	        = -99
		*/
	}

	std::ostream& operator <<(std::ostream & out, const ProName & name) {
		out << static_cast< const wchar_t* >(name);
		return out;
	}

	std::ostream& operator <<(std::ostream & out, const ProParamvalue & param) {
		switch ( param.type ) {
		case PRO_PARAM_DOUBLE:
			out << " DOUBLE: " << param.value.d_val;
			return out;
		case PRO_PARAM_STRING:
			out << " STRING: " << param.value.s_val;
			return out;
		case PRO_PARAM_INTEGER:
			out << " INTEGER: " << param.value.i_val;
			return out;
		case PRO_PARAM_BOOLEAN:
			out << " BOOLEAN: " << param.value.l_val;
			return out;
		case PRO_PARAM_NOTE_ID:
			out << " NOTE: " << param.value.i_val;
		case PRO_PARAM_VOID:
			out << " VOID ";
			return out;
		case PRO_PARAM_NOT_SET:
			out << " NOT-SET ";
			return out;
		default:
			out << " UNKNOWN ";
		    return out;
		}
	}

	std::ostream& operator <<(std::ostream & out, const ProMdl & model) {
		out << static_cast<const void *>(model);
		return out;
	}

	std::ostream& operator <<(std::ostream & out, ProMdl & model) {
		out << static_cast<const void *>(model);
		return out;
	}

	std::ostream& operator <<(std::ostream & out, const ProModel & model) {
		out << " Model { " << " name: " << model.name << " type: " << model.type << " } ";
		return out;
	}
	
	std::ostream& operator <<(std::ostream & out, const ProModelitem & item) {
		out << " id: " << item.id << " type: " << item.type;
		
		switch ( item.type ) {
		case PRO_TYPE_DIR:
			break;
		case PRO_TYPE_INVALID:
		case PRO_TYPE_UNUSED:
			break;
		case PRO_ASSEMBLY:
			out << " owner: {" << item.owner << "}";
			break;
		case PRO_PART:
			out << " owner: {" << item.owner << "}";
			break;
		case PRO_FEATURE:
			out << " owner: {" << item.owner << "}";
			break;
		default:
			out << " owner: {" << item.owner << "}";
		}
		
		return out;
	}

	std::ostream& operator <<(std::ostream & out, const ProMdlType & type) {
		switch ( type ) {
		case PRO_MDL_UNUSED:
			out << " PRO_MDL_UNUSED ";
			return out;
		case PRO_MDL_ASSEMBLY:
			out << " PRO_MDL_ASSEMBLY ";
			return out;
		case PRO_MDL_PART:
			out << " PRO_MDL_PART ";
			return out;
		case PRO_MDL_DRAWING:
			out << " PRO_MDL_DRAWING ";
			return out;
		case PRO_MDL_3DSECTION:
			out << " PRO_MDL_3DSECTION ";
			return out;
		case PRO_MDL_2DSECTION:
			out << " PRO_MDL_2DSECTION ";
			return out;
		case PRO_MDL_LAYOUT:
			out << " PRO_MDL_LAYOUT ";
			return out;
		case PRO_MDL_DWGFORM:
			out << " PRO_MDL_DWGFORM ";
			return out;
		case PRO_MDL_MFG:
			out << " PRO_MDL_MFG ";
			return out;
		case PRO_MDL_REPORT:
			out << " PRO_MDL_REPORT ";
			return out;
		case PRO_MDL_MARKUP:
			out << " PRO_MDL_MARKUP ";
			return out;
		case PRO_MDL_DIAGRAM:
			out << " PRO_MDL_DIAGRAM ";
			return out;
		default:
			out << "MODEL TYPE" << static_cast<int>(type); 
		    return out;
		}
	}

	std::ostream& operator <<(std::ostream & out, const ProParamowner & owner) {
		switch ( owner.type ) {
		case  PRM_MODEL:
			out << " MODEL: " << owner.who.model;
			return out;
		case PRM_ITEM:
			out << " ITEM: " << owner.who.item;
			return out;
		}
		return out;
	}

	
	std::ostream& operator <<(std::ostream & out, const ProParameter & param) {
		out << param.type << param.id << param.owner;
		return out;
	}

	/**
	see "ProObjects.h"
	*/
	std::ostream& operator <<(std::ostream & out, const ProType & type) {
		switch ( type ) {
		case PRO_TYPE_DIR: out << "PRO_TYPE_DIR"; break;
		case PRO_TYPE_INVALID: out << "PRO_TYPE_INVALID"; break;
		
		case PRO_TYPE_UNUSED :  out << "PRO_TYPE_UNUSED"; break;
		case PRO_ASSEMBLY :     out << "PRO_ASSEMBLY"; break;
		case PRO_PART :         out << "PRO_PART"; break;
		case PRO_FEATURE :      out << "PRO_FEATURE"; break;
		case PRO_DRAWING :      out << "PRO_DRAWING"; break;
		case PRO_SURFACE :      out << "PRO_SURFACE"; break;
		case PRO_EDGE :         out << "PRO_EDGE"; break;
		case PRO_3DSECTION :    out << "PRO_3DSECTION"; break;
		case PRO_DIMENSION :    out << "PRO_DIMENSION"; break;
		case PRO_2DSECTION :    out << "PRO_2DSECTION"; break;
		case PRO_PAT_MEMBER :   out << "PRO_PAT_MEMBER"; break;
		case PRO_PAT_LEADER :   out << "PRO_PAT_LEADER"; break;
	  /*
	  PRO_XSEC                =   18,
	  PRO_LAYOUT              =   19,
	  PRO_AXIS                =   21,
	  PRO_CSYS                =   25,
	  PRO_REF_DIMENSION       =   28,
	  PRO_GTOL                =   32,
	  PRO_DWGFORM             =   33,
	  PRO_SUB_ASSEMBLY        =   34,
	  PRO_MFG                 =   37,
	  PRO_SURF_FIN            =   42,
	  PRO_QUILT               =   57,
	  PRO_CURVE               =   62,
	  PRO_POINT               =   66,
	  PRO_NOTE                =   68,
	  PRO_IPAR_NOTE           =   69,
	  PRO_EDGE_START          =   71,
	  PRO_EDGE_END            =   72,
	  PRO_CRV_START           =   74,
	  PRO_CRV_END             =   75,
	  PRO_SYMBOL_INSTANCE     =   76,
	  PRO_DRAFT_ENTITY        =   77,
	  PRO_DRAFT_DATUM         =   79,
	  PRO_DRAFT_GROUP         =   83,
	  PRO_DRAW_TABLE          =   84,
	  PRO_COSMETIC            =   90,
	  PRO_VIEW                =   92,
	  PRO_CABLE               =   96,
	  PRO_REPORT              =  105,
	  PRO_MARKUP              =  116,
	  PRO_LAYER               =  117,
	  PRO_DIAGRAM             =  121,
	  PRO_SKETCH_ENTITY       =  133,
	  PRO_DATUM_TEXT          =  144,
	  PRO_ENTITY_TEXT         =  145,
	  PRO_DRAW_TABLE_CELL     =  147,
	  PRO_PIPE_SEG            =  174,
	  PRO_DATUM_PLANE         =  176,
	  PRO_COMP_CRV            =  180,
	  PRO_BND_TABLE           =  211,
	  PRO_ANNOTATION_ELEM     =  219,
	  PRO_SET_DATUM_TAG       =  220,
	  PRO_ANNOT_ELEM_DRIVING_DIM =  224,
	  PRO_PARAMETER           =  240,
	  PRO_SILH_EDGE           =  256,
	  PRO_DIAGRAM_OBJECT      =  305,
	  PRO_DIAGRAM_WIRE        =  308,
	  PRO_SIMP_REP            =  309,
	  PRO_CE_DRAWING          =  315,
	  PRO_WELD_PARAMS         =  371,
	  PRO_SNAP_LINE           =  377,
	  PRO_EXTOBJ              =  385,
	  PRO_CSYS_AXIS_X         =  407,
	  PRO_CSYS_AXIS_Y         =  408,
	  PRO_CSYS_AXIS_Z         =  409,
	  PRO_STYLE_STATE         =  460,
	  PRO_VIS_STATE           =  461,
	  PRO_COLOR_STATE         =  462,
	  PRO_COMBINED_STATE      =  463,
	  PRO_REFONLY_STATE       =  464,
	  PRO_LAYER_STATE         =  465,
	  PRO_SUBSET_STATE        =  466,
	  PRO_EXPLD_STATE         =  500,
	  PRO_CABLE_LOCATION      =  504,
	  PRO_RELSET              =  533,
	  PRO_ANALYSIS            =  555,
	  PRO_SURF_CRV            =  556,
	  PRO_SOLID_GEOMETRY      =  622,
	  PRO_LOG_SRF             =  625,
	  PRO_LOG_EDG             =  626,
	  PRO_DESKTOP             =  627,
	  PRO_SYMBOL_DEFINITION   =  628,
	  PRO_FACET_SET           =  630,
	  PRO_LOG_OBJECT          =  640,
	  PRO_NEUTRAL_LAYER       =  641,
	  PRO_IC_START            =  642,
	  PRO_IC_END              =  643,
	  PRO_EDGE_PNT            =  660,
	  PRO_CRV_PNT             =  661,
	  PRO_EDGE_END_PNT        =  662,
	  PRO_NC_STEP_MODEL       =  804,
	  PRO_NC_STEP_OBJECT      =  805,
	  PRO_ANNOT_PLANE         =  849,
	  PRO_CUSTOM_ANNOTATION   =  850,
	  PRO_TOOL_MOTION         =  907,
	  PRO_LOG_PNT             =  913,
	  PRO_LOG_PLANE           =  914,
	  PRO_LOG_CSYS            =  915,
	  PRO_LOG_AXIS            =  916, 
	  PRO_SURFACE_PNT         =  919,

	  PRO_SURF_REGION_SIDE1   =  933,
	  PRO_SURF_REGION_SIDE2   =  934,
	  PRO_SRF_PLANE_PNT       =  935,
	  PRO_CRV_SIDE1SRF_CNTR   =  937,
	  PRO_CRV_SIDE2SRF_CNTR   =  938,

	  PRO_SKETCH_CONSTRAINT   =  942,

	  PRO_CONTOUR             = 1000,
	  PRO_GROUP               = 1001,
	  PRO_UDF                 = 1002,
	  PRO_FAMILY_TABLE        = 1003,
	  PRO_PATREL_FIRST_DIR    = 10018,
	  PRO_PATREL_SECOND_DIR   = 10019,
	  PRO_JAR_FILE            =  10020,

	  PRO_SIMULATION_LOAD          = 11000,
	  PRO_SIMULATION_WCS           = 11001,
	  PRO_SIMULATION_BEAM          = 11004,
	  PRO_SIMULATION_SHELL         = 11005,
	  PRO_SIMULATION_BEAM_SECTION  = 11007,
	  PRO_SIMULATION_BEAM_ORIENT   = 11008,
	  PRO_SIMULATION_BEAM_RELEASE  = 11009,
	  PRO_SIMULATION_SHELL_PROPS   = 11010,
	  PRO_SIMULATION_MATL_ORIENT   = 11011,
	  PRO_SIMULATION_SPRING        = 11012,
	  PRO_SIMULATION_SPRING_PROPS  = 11013,
	  PRO_SIMULATION_GAP           = 11014,
	  PRO_SIMULATION_CONTACT       = PRO_SIMULATION_GAP,
	  PRO_SIMULATION_MASS          = 11015,
	  PRO_SIMULATION_MASS_PROPS    = 11016,
	  PRO_SIMULATION_MESH_CNTRL    = 11017,
	  PRO_SIMULATION_LOAD_SET      = 11018,
	  PRO_SIMULATION_FUNCTION      = 11019,
	  PRO_SIMULATION_CONSTRAINT    = 11020,
	  PRO_SIMULATION_CONSTR_SET    = 11021,
	  PRO_SIMULATION_SHELL_PAIR    = 11022,
	  PRO_SIMULATION_CONNECTION    = 11023,
	  PRO_SIMULATION_INTERFACE     = PRO_SIMULATION_CONNECTION,
	  PRO_SIMULATION_WELD          = 11024,
	  PRO_SIMULATION_MATL_ASSIGN   = 11025,
	  PRO_SIMULATION_MEASURE       = 11026,
	  PRO_SIMULATION_RUNNER        = 11027,  //obsolete
	  PRO_SIMULATION_ENTRANCE_PNT  = 11028,  //obsolete
	  PRO_SIMULATION_STIFF_COND    = 11029,
	  PRO_SIMULATION_RIGID_LINK    = 11030,
	  PRO_SIMULATION_WEIGHT_LINK   = 11033,
	  PRO_SIMULATION_BOLT          = 11035,
	  PRO_SIMULATION_CONT_REGION   = 11036,  // obsolete 
	  PRO_SIMULATION_OBJECT        = 11037,
	  PRO_SIMULATION_ANALYSIS      = 11038,

	  PRO_SIMULATION_VOLUME        = 11200,
	  PRO_SIMULATION_PNT_PATTERN   = 11201,
	  PRO_SIMULATION_FEAT_SEC      = 11202,
	  PRO_SIMULATION_HP_FACE       = 11203,
	  PRO_SIMULATION_HP_EDGE       = 11204,
	  PRO_SIMULATION_HPE_BUNDLE    = 11205,

	  PRO_MESH_MESH                = 11999,
	  PRO_MESH_COMPONENT           = 12000,
	  PRO_MESH_SURFACE             = 12002,
	  PRO_MESH_EDGE                = 12004,
	  PRO_MESH_CURVE               = 12005,
	  PRO_MESH_VERTEX              = 12006,
	  PRO_MESH_HARD_POINT          = 12008,
	  PRO_MESH_NODE                = 12020,
	  PRO_MESH_ELEMENT             = 12021,

	  PRO_FEM_NEUTRAL_FILE         = 414,
	  PRO_FEM_TM_FILE              = 12900,
	  PRO_MECH_DIR                 = 12905,

	  PRO_DISPOBJ                  = 13000,

	  PRO_RP_MATERIAL              = 17001,

	  PRO_RP_FUNCTION              = 17002,

	  PRO_QUERY                    = 45106,

	  PRO_ARTWORK                  = 55100,  // PI Artwork 

	  PRO_MDO_BODY                 = 70000,
	  PRO_MDO_CAM_CONN             = 70003,
	  PRO_MDO_GEAR_CONN            = 70004,
	  PRO_MDO_SERVO_MOTOR          = 70005,
	  PRO_MDO_FORCE_MOTOR          = 70006,
	  PRO_MDO_SPRING               = 70007,
	  PRO_MDO_DAMPER               = 70008,
	  PRO_MDO_FORCE                = 70009,
	  PRO_MDO_TORQUE               = 70010,
	  PRO_MDO_ANALYSIS             = 70011,
	  PRO_MDO_SNAPSHOT             = 70012,
	  PRO_MDO_INIT_COND            = 70013,
	  PRO_MDO_MEASURE              = 70014,
	  PRO_MDO_JAS                  = 70015,
	  PRO_MDO_GRAVITY              = 70016,
	  PRO_MDO_MASSPROP             = 70017,
	  PRO_MDO_SETTINGS             = 70018,
	  PRO_MDO_CONN                 = 70019,
	  PRO_MDO_CONN_AXIS            = 70020,    // obsoleted 
	  PRO_MDO_SLOT_CONN            = 70021,
	  PRO_MDO_CONN_PARAM           = 70022,
	  PRO_MDO_LOAD_XFER            = 70023,
	  PRO_MDO_SLOT_AXIS            = 70024,
	  PRO_MDO_CONN_AXIS_TR_1       = 70025,
	  PRO_MDO_CONN_AXIS_TR_2       = 70026,
	  PRO_MDO_CONN_AXIS_TR_3       = 70027,
	  PRO_MDO_CONN_AXIS_ROT_1      = 70028,
	  PRO_MDO_CONN_AXIS_ROT_2      = 70029,
	  PRO_MDO_CONN_AXIS_ROT_3      = 70030,
	  PRO_MDO_CONN_AXIS_EXT        = 70031,
	  PRO_MDO_CONTACT_3D           = 70032,
	  PRO_MDO_BELT                 = 70033,
	  PRO_MDO_BUSHING_LD           = 70034,
	  PRO_MDO_CONN_AXIS_CONE       = 70035,
	  PRO_MDO_TERM_COND            = 70036,
  
	  PRO_PLACEMENT_SET            = 70100,

	  PRO_COLSN_DATA               = 71000,

	  PRO_ANIM_ANIMATION           = 73000,
	  PRO_ANIM_SUB_ANIMATION       = 73001,
	  PRO_ANIM_BODY                = 73002,
	  PRO_ANIM_BODY_LOCK           = 73003,
	  PRO_ANIM_CONN_STATUS         = 73004,
	  PRO_ANIM_EVENT               = 73005,
	  PRO_ANIM_DRIVER_INSTANCE     = 73006,
	  PRO_ANIM_KFS                 = 73007,
	  PRO_ANIM_KFS_INSTANCE        = 73008,
	  PRO_ANIM_VIEW_AT_TIME        = 73009,
	  PRO_ANIM_DISPLAY_AT_TIME     = 73010,
	  PRO_ANIM_TRANS_AT_TIME       = 73011,
	  PRO_ANIM_COMB                = 73012,
	  PRO_ANIM_PI_KFS_INSTANCE     = 73013, // obsoleted 
	  PRO_ANIM_PI_INT_PT           = 73014, // obsoleted 
	  PRO_ANIM_PI_INT_AXIS         = 73015, // obsoleted 
	  PRO_ANIM_PI_INT_PLANE        = 73016, // obsoleted 
	  PRO_ANIM_EXPLD_KFS           = 73017,
	  PRO_ANIM_EXPLD_KFS_INSTANCE  = 73018,
	  PRO_ANIM_EXPLD_SUB_ANIMATION = 73019,
	  PRO_ANIM_EXPLD_EVENT         = 73020,
	  PRO_ANIM_MDO_EVENT           = 73021,
	  PRO_ANIM_MDO_MOVIE           = 73022,
	  PRO_ANIM_EXPLD_COMB          = 73023,
	  PRO_ANIM_SNAP_COMB_KFS       = 73024,
	  PRO_ANIM_SNAP_COMB_KFS_INSTANCE = 73025,
	  PRO_ANIM_EXPLD_COMB_KFS       = 73026,
	  PRO_ANIM_EXPLD_COMB_KFS_INSTANCE = 73027,

	  PRO_LOG_CURVE                = 74150,
	  PRO_LOG_COLLECTION           = 74151
	  */
     default: 
			out << "[" << static_cast<int>(type) << "]"; 
     }
  return out;
  }
  
}