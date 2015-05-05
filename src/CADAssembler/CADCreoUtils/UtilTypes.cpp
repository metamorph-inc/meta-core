

/*--------------------------------------------------------------------*\
Pro/TOOLKIT includes
\*--------------------------------------------------------------------*/
#include        <ProToolkit.h>
#include        <ProObjects.h>
#include        <ProMdl.h>
#include        <ProMfg.h>
#include        <ProFeature.h>
#include        <ProGeomitem.h>
#include        <ProFeatType.h>
#include        <ProScope.h>
#include        <ProGtol.h>
#include        "UtilString.h"
#include        <ProTKRunTime.h>

#define TABLE_TERM	"TABLE_END"

typedef struct  type_to_str
{
    int type;
    char str[PRO_NAME_SIZE];
} Type_to_str;

static Type_to_str feattype_table[] = {
        {PRO_FEAT_FIRST_FEAT,		"FIRST_FEAT"	},
        {PRO_FEAT_HOLE,      		"HOLE"		},
        {PRO_FEAT_SHAFT,		"SHAFT"		},
        {PRO_FEAT_ROUND,		"ROUND"		},
        {PRO_FEAT_CHAMFER,		"CHAMFER"	},
        {PRO_FEAT_SLOT,			"SLOT"		},
        {PRO_FEAT_CUT,			"CUT"		},
        {PRO_FEAT_PROTRUSION,		"PROTRUSION"	},
        {PRO_FEAT_NECK,			"NECK"		},
        {PRO_FEAT_FLANGE,		"FLANGE"	},
        {PRO_FEAT_RIB,			"RIB"		},
        {PRO_FEAT_EAR,			"EAR"		},
        {PRO_FEAT_DOME,			"DOME"		},
        {PRO_FEAT_DATUM,		"DATUM"		},
        {PRO_FEAT_LOC_PUSH,		"LOC_PUSH"	},
        {PRO_FEAT_UDF,			"UDF"		},
        {PRO_FEAT_DATUM_AXIS,		"AXIS"		},
        {PRO_FEAT_DRAFT,		"DRAFT"		},
        {PRO_FEAT_SHELL,		"SHELL"		},
        {PRO_FEAT_DOME2,		"DOME2"		},
        {PRO_FEAT_CORN_CHAMF,		"CORN_CHAMF"	},
        {PRO_FEAT_DATUM_POINT,		"DATUM_POINT"	},
        {PRO_FEAT_IMPORT,		"IMPORT"	},
        {PRO_FEAT_IGES,			"IGES"		},
        {PRO_FEAT_COSMETIC,		"COSMETIC"	},
        {PRO_FEAT_ETCH,			"ETCH"		},
        {PRO_FEAT_GEN_MERGE,		"MERGE"		},
        {PRO_FEAT_MERGE,		"MERGE"		},
        {PRO_FEAT_MOLD,			"MOLD"		},
        {PRO_FEAT_SAW,			"SAW"		},
        {PRO_FEAT_TURN,			"TURN"		},
        {PRO_FEAT_MILL,			"MILL"		},
        {PRO_FEAT_DRILL,		"DRILL"		},
        {PRO_FEAT_OFFSET,		"OFFSET"	},
        {PRO_FEAT_DATUM_SURF,		"DATUM_SURF"	},
        {PRO_FEAT_REPLACE_SURF,		"REPLACE_SURF"	},
        {PRO_FEAT_GROOVE,		"GROOVE"	},
        {PRO_FEAT_PIPE,			"PIPE"		},
        {PRO_FEAT_DATUM_QUILT,		"DATUM_QUILT"	},
        {PRO_FEAT_ASSEM_CUT,		"ASSEM_CUT"	},
        {PRO_FEAT_UDF_THREAD,		"UDF_THREAD"	},
        {PRO_FEAT_CURVE,		"CURVE"		},
        {PRO_FEAT_SRF_MDL,		"SRF_MDL"	},
        {PRO_FEAT_WALL,			"WALL"		},
        {PRO_FEAT_BEND,			"BEND"		},
        {PRO_FEAT_UNBEND,		"UNBEND"	},
        {PRO_FEAT_CUT_SMT,		"CUT_SMT"	},
        {PRO_FEAT_FORM,			"FORM"		},
        {PRO_FEAT_PUNCH_QUILT,          "PUNCH_QUILT"   },
        {PRO_FEAT_THICKEN,		"THICKEN"	},
        {PRO_FEAT_BEND_BACK,		"BEND_BACK"	},
        {PRO_FEAT_UDF_NOTCH,		"UDF_NOTCH"	},
        {PRO_FEAT_UDF_PUNCH,		"UDF_PUNCH"	},
        {PRO_FEAT_INT_UDF,		"INT_UDF"	},
        {PRO_FEAT_SPLIT_SURF,		"SPLIT_SURF"	},
        {PRO_FEAT_GRAPH,		"GRAPH"		},
        {PRO_FEAT_SMT_MFG_PUNCH,	"SMT_MFG_PUNCH"	},
        {PRO_FEAT_SMT_MFG_CUT,		"SMT_MFG_CUT"	},	
        {PRO_FEAT_FLATTEN,		"FLATTEN"	},	
        {PRO_FEAT_SET,			"SET"		},
        {PRO_FEAT_VDA,			"VDA"		},
        {PRO_FEAT_SMT_MFG_FORM,		"SMT_MFG_FORM"	},
        {PRO_FEAT_SMT_PUNCH_PNT,	"SMT_PUNCH_PNT"	},
        {PRO_FEAT_LIP,			"LIP"		},
        {PRO_FEAT_MANUAL,		"MANUAL"	},
        {PRO_FEAT_MFG_GATHER,		"MFG_GATHER"	},
        {PRO_FEAT_MFG_TRIM,		"MFG_TRIM"	},
        {PRO_FEAT_MFG_USEVOL,		"MFG_USEVOL"	},
        {PRO_FEAT_LOCATION,		"LOCATION"	},
        {PRO_FEAT_CABLE_SEGM,		"CABLE_SEGM"	},
        {PRO_FEAT_CABLE,		"CABLE"		},
        {PRO_FEAT_CSYS,			"CSYS"		},
        {PRO_FEAT_CHANNEL,		"CHANNEL"	},
        {PRO_FEAT_WIRE_EDM,		"WIRE_EDM"	},
        {PRO_FEAT_AREA_NIBBLE,		"AREA_NIBBLE"	},
        {PRO_FEAT_PATCH,		"PATCH"		},
        {PRO_FEAT_PLY,			"PLY"		},
        {PRO_FEAT_CORE,			"CORE"		},
        {PRO_FEAT_EXTRACT,		"EXTRACT"	},
        {PRO_FEAT_MFG_REFINE,		"MFG_REFINE"	},
        {PRO_FEAT_SILH_TRIM,		"SILH_TRIM"	},
        {PRO_FEAT_SPLIT,		"SPLIT"		},
        {PRO_FEAT_EXTEND,		"EXTEND"	},
        {PRO_FEAT_SOLIDIFY,		"SOLIDIFY"	},
        {PRO_FEAT_INTERSECT,		"INTERSECT"	},
        {PRO_FEAT_ATTACH,		"ATTACH"	},
        {PRO_FEAT_XSEC,			"XSEC"		},
        {PRO_FEAT_UDF_ZONE,		"UDF_ZONE"	},
        {PRO_FEAT_UDF_CLAMP,		"UDF_CLAMP"	},
        {PRO_FEAT_DRL_GRP,		"DRL_GRP"	},
        {PRO_FEAT_ISEGM,		"ISEGM"		},
        {PRO_FEAT_CABLE_COSM,		"CABLE_COSM"	},
        {PRO_FEAT_SPOOL,		"SPOOL"		},
        {PRO_FEAT_COMPONENT,		"COMPONENT"	},
        {PRO_FEAT_MFG_MERGE,		"MFG_MERGE"	},
        {PRO_FEAT_FIXSETUP,		"FIXSETUP"	},
        {PRO_FEAT_SETUP,		"SETUP"		},
        {PRO_FEAT_FLAT_PAT,		"FLAT_PAT"	},
        {PRO_FEAT_CONT_MAP,		"CONT_MAP"	},
        {PRO_FEAT_EXP_RATIO,		"EXP_RATIO"	},
        {PRO_FEAT_RIP,			"RIP"		},
        {PRO_FEAT_OPERATION,		"OPERATION"	},
        {PRO_FEAT_WORKCELL,		"WORKCELL"	},
        {PRO_FEAT_CUT_MOTION,		"CUT_MOTION"	},
        {PRO_FEAT_BLD_PATH,		"BLD_PATH"	},
        {PRO_FEAT_DRV_TOOL_SKETCH,	"DRV_TOOL_SKETCH"	},
        {PRO_FEAT_DRV_TOOL_EDGE,	"DRV_TOOL_EDGE"	},
        {PRO_FEAT_DRV_TOOL_CURVE,	"DRV_TOOL_CURVE"	},
        {PRO_FEAT_DRV_TOOL_SURF,	"DRV_TOOL_SURF"	},
        {PRO_FEAT_MAT_REMOVAL,		"MAT_REMOVAL"	},
        {PRO_FEAT_TORUS,		"TORUS"		},
        {PRO_FEAT_PIPE_SET_START,	"PIPE_SET_START"	},
        {PRO_FEAT_PIPE_PNT_PNT,		"PIPE_PNT_PNT"	},
        {PRO_FEAT_PIPE_EXT,		"PIPE_EXT"	},
        {PRO_FEAT_PIPE_TRIM,		"PIPE_TRIM"	},
        {PRO_FEAT_PIPE_FOLL,		"PIPE_FOLL"	},
        {PRO_FEAT_PIPE_JOIN,		"PIPE_JOIN"	},
        {PRO_FEAT_AUXILIARY,		"AUXILIARY"	},
        {PRO_FEAT_PIPE_LINE,		"PIPE_LINE"	},
        {PRO_FEAT_LINE_STOCK,		"LINE_STOCK"	},
        {PRO_FEAT_SLD_PIPE,		"SLD_PIPE"	},
        {PRO_FEAT_BULK_OBJECT,		"BULK_OBJECT"	},
        {PRO_FEAT_SHRINKAGE,		"SHRINKAGE"	},
        {PRO_FEAT_PIPE_JOINT,		"PIPE_JOINT"	},
        {PRO_FEAT_PIPE_BRANCH,		"PIPE_BRANCH"	},
        {PRO_FEAT_DRV_TOOL_TWO_CNTR,	"DRV_TOOL_TWO_CNTR"	},
        {PRO_FEAT_SUBHARNESS,		"SUBHARNESS"	},
        {PRO_FEAT_SUBHARNESS,		"SUBHARNESS"	},
        {PRO_FEAT_SMT_OPTIMIZE,		"SMT_OPTIMIZE"	},
        {PRO_FEAT_DECLARE,		"DECLARE"	},
        {PRO_FEAT_SMT_POPULATE,		"SMT_POPULATE"	},
        {PRO_FEAT_OPER_COMP,		"OPER_COMP"	},
        {PRO_FEAT_MEASURE,		"MEASURE"	},
        {PRO_FEAT_DRAFT_LINE,   	"DRAFT_LINE"	},
        {PRO_FEAT_REMOVE_SURFS,		"REMOVE_SURFS"	},
        {PRO_FEAT_RIBBON_CABLE,		"RIBBON_CABLE"	},
        {PRO_FEAT_ATTACH_VOLUME,	"ATTACH_VOLUME"	},
        {PRO_FEAT_BLD_OPERATION,	"BLD_OPERATION"	},
        {PRO_FEAT_UDF_WRK_REG,		"UDF_WRK_REG"	},
        {PRO_FEAT_SPINAL_BEND,		"SPINAL_BEND"	},
        {PRO_FEAT_TWIST,		"TWIST"		},
        {PRO_FEAT_FREE_FORM,		"FREE_FORM"	},
        {PRO_FEAT_ZONE,			"ZONE"		},
        {PRO_FEAT_WELDING_ROD,		"WELDING_ROD"	},
        {PRO_FEAT_WELD_FILLET,		"WELD_FILLET"	},
        {PRO_FEAT_WELD_GROOVE,		"WELD_GROOVE"	},
        {PRO_FEAT_WELD_PLUG_SLOT,	"WELD_PLUG_SLOT"	},
        {PRO_FEAT_WELD_SPOT,		"WELD_SPOT"	},
        {PRO_FEAT_SMT_SHEAR,		"SMT_SHEAR"	},
        {PRO_FEAT_PATH_SEGM,		"PATH_SEGM"	},
        {PRO_FEAT_RIBBON_SEGM,		"RIBBON_SEGM"	},
        {PRO_FEAT_RIBBON_PATH,		"RIBBON_PATH"	},
        {PRO_FEAT_RIBBON_EXTEND,	"RIBBON_EXTEND"	},
        {PRO_FEAT_ASMCUT_COPY,		"ASMCUT_COPY"	},
        {PRO_FEAT_DEFORM_AREA,		"DEFORM_AREA"	},
        {PRO_FEAT_RIBBON_SOLID,		"RIBBON_SOLID"	},
        {PRO_FEAT_FLAT_RIBBON_SEGM,	"FLAT_RIBBON_SEGM"	},
        {PRO_FEAT_POSITION_FOLD,	"POSITION_FOLD"	},
	/*
        {PRO_FEAT_SPRING_BACK,		"SPRING_BACK"	},
	*/
        {PRO_FEAT_BEAM_SECTION,		"BEAM_SECTION"	},
        {PRO_FEAT_SHRINK_DIM,		"SHRINK_DIM"	},	
        {PRO_FEAT_UDF_THREAD,		"THREAD"	},
        {PRO_FEAT_USER_FEAT,		"USER_FEAT"	},
        {-1,				TABLE_TERM	}
};

static Type_to_str objtype_table[] = {
        {PRO_TYPE_UNUSED,	"TYPE_UNUSED"	},
        {PRO_ASSEMBLY,		"ASSEMBLY"	},
        {PRO_PART,		"PART"		},
        {PRO_FEATURE,		"FEATURE"	},
        {PRO_DRAWING,		"DRAWING"	},
        {PRO_SURFACE,		"SURFACE"	},
        {PRO_EDGE,		"EDGE"		},
        {PRO_AXIS,		"AXIS"		},
        {PRO_CSYS,		"CSYS"		},
        {PRO_MFG,		"MFG"		},
        {PRO_QUILT,		"QUILT"		},
        {PRO_CURVE,		"CURVE"		},
        {PRO_POINT,		"POINT"		},
        {PRO_EDGE_START,	"EDGE_START"	},
        {PRO_EDGE_END,		"EDGE_END"	},
        {PRO_CRV_START,		"CRV_START"	},
        {PRO_CRV_END,		"CRV_END"	},
        {PRO_COMP_CRV,		"COMP_CRV"	},
        {PRO_PARAMETER,		"PARAMETER"	},
        {PRO_CONTOUR,		"CONTOUR"	},
        {PRO_GROUP,		"GROUP"		},
        {-1,			TABLE_TERM	}
};

static Type_to_str surftype_table[] = {
        {PRO_SRF_NONE,		"Undefined"	},
        {PRO_SRF_PLANE,		"Plane"	},
        {PRO_SRF_CYL,		"Cylinder"	},
        {PRO_SRF_CONE,		"Cone"	},
        {PRO_SRF_TORUS,		"Torus"	},
        {PRO_SRF_COONS,		"Coons patch"	},
        {PRO_SRF_SPL,		"Spline"	},
        {PRO_SRF_FIL,		"Fillet"	},
        {PRO_SRF_RUL,		"Ruled"	},
        {PRO_SRF_REV,		"Revolved"	},
        {PRO_SRF_TABCYL,	"Tubulated Cylinder"	},
        {PRO_SRF_B_SPL,		"B Spline"	},
        {PRO_SRF_FOREIGN,	"Foreign"	},
        {PRO_SRF_CYL_SPL,	"Cylindrical Spline"	},
        {-1,			TABLE_TERM	}
};

static Type_to_str enttype_table[] = {
        {PRO_ENT_NONE,		"Undefined"	},
        {PRO_ENT_POINT,		"Point"		},
        {PRO_ENT_LINE,		"Line"		},
        {PRO_ENT_ARC,		"Arc"		},
        {PRO_ENT_SPLINE,	"Spline"	},
        {PRO_ENT_B_SPLINE,	"B Spline"	},
        {PRO_ENT_ELLIPSE,	"Ellipse"	},
        {PRO_ENT_POLYGON,	"Polygon"	},
        {PRO_ENT_CMP_CRV,	"Composite"	},
        {-1,			TABLE_TERM	}
};

static Type_to_str mfgtype_table[] = {
	{PRO_MFGTYPE_MACH_PART, "Part"},
	{PRO_MFGTYPE_MACH_ASSEM, "Assembly"},
	{PRO_MFGTYPE_SHEET_METAL, "Sheetmetal"},
	{PRO_MFGTYPE_MOLD, "Mold"},
	{PRO_MFGTYPE_CAST, "Cast"},
	{PRO_MFGTYPE_CMM, "CMM"},
	{PRO_MFGTYPE_NONE, TABLE_TERM}
};

static Type_to_str proextrefscore_table[]={
    {PRO_REFCTRL_ALLOW_ALL,         "All"},
    {PRO_REFCTRL_ALLOW_SUBASSEMBLY, "Subassembly"},
    {PRO_REFCTRL_ALLOW_SKELETON,    "Skeleton"},
    {PRO_REFCTRL_ALLOW_NONE,        "None"},
    {-1,                            TABLE_TERM}        
};

static Type_to_str progtol_tbl[] ={
   {PROGTOLTYPE_UNKNOWN,        "Unknown"},
   {PROGTOLTYPE_STRAIGHTNESS,   "Straightness"},
   {PROGTOLTYPE_FLATNESS,       "Flatness"},
   {PROGTOLTYPE_CIRCULAR,       "Circular"},
   {PROGTOLTYPE_CYLINDRICAL,    "Cylindrical"},
   {PROGTOLTYPE_LINE,           "Line"},
   {PROGTOLTYPE_SURFACE,        "Surface"},
   {PROGTOLTYPE_ANGULAR,        "Angular"},
   {PROGTOLTYPE_PERPENDICULAR,  "Perpendicular"},
   {PROGTOLTYPE_PARALLEL,       "Parallel"},
   {PROGTOLTYPE_POSITION,       "Position"},
   {PROGTOLTYPE_CONCENTRICITY,  "Concentricity"},
   {PROGTOLTYPE_SYMMETRY,       "Symmetry"},
   {PROGTOLTYPE_CIRCULAR_RUNOUT,"Circular run out"},
   {PROGTOLTYPE_TOTAL_RUNOUT,   "Total run out"},
   {-1,                          TABLE_TERM}
};

static Type_to_str progtol_materialcond_tbl[] ={
    {PROGTOLMATCOND_UNKNOWN,    "UUnKnown"},
    {PROGTOLMATCOND_LMC,        "LMC"},
    {PROGTOLMATCOND_MMC,        "MMC"},
    {PROGTOLMATCOND_RFS,        "RFS"},
    {PROGTOLMATCOND_DEFAULT_RFS,"Default - RFS"},
    {-1,                         TABLE_TERM}
};

static Type_to_str progtol_leader_tbl[]={
    {PROLEADERTYPE_ARROWHEAD,  "Arrow heaad"},
    {PROLEADERTYPE_DOT,        "Dot"},
    {PROLEADERTYPE_FILLEDDOT,  "Filled dot"},
    {PROLEADERTYPE_NOARROW,    "No arrow"},            
    {PROLEADERTYPE_SLASH,      "Slash"},
    {PROLEADERTYPE_INTEGRAL,   "Integral"},
    {PROLEADERTYPE_BOX,        "Box"},
    {PROLEADERTYPE_FILLEDBOX,  "Filled box"},
    {PROLEADERTYPE_DOUBLEARROW,"Double arrow"},
    {-1,                        TABLE_TERM}
};

static Type_to_str progtol_placement_tbl[]={
    {PROGTOLPTYPE_DATUM,       "Datum"},
    {PROGTOLPTYPE_DIMENSION,   "Dimension"},
    {PROGTOLPTYPE_DIM_RELATED, "Dimension related"},
    {PROGTOLPTYPE_FREENOTE,    "Free note"},
    {PROGTOLPTYPE_LEADERS,     "Leaders"}, 
    {PROGTOLPTYPE_TANLEADER,   "Tanleader"},
    {PROGTOLPTYPE_NORMLEADER,  "Normleader"},
    {PROGTOLPTYPE_GTOL,        "Geometric tolerance"},               
    {-1,                        TABLE_TERM}
};

static Type_to_str progtol_refitem_tbl[]={
    {PROGTOLRTYPE_NONE,   "None"},
    {PROGTOLRTYPE_EDGE,   "Edge"},
    {PROGTOLRTYPE_AXIS,   "Axis"},
    {PROGTOLRTYPE_SURF,   "Surface"},
    {PROGTOLRTYPE_FEAT,   "Feature"},
    {PROGTOLRTYPE_DATUM,  "Datum"},
    {PROGTOLRTYPE_ENTITY, "Entity"},
    {-1,                   TABLE_TERM}
}; 

static Type_to_str progtol_reftype_tbl[]={
    {PROGTOLREFTYPE_NONE,     "None"},
    {PROGTOLREFTYPE_PRIMARY,  "Primary"},
    {PROGTOLREFTYPE_SECONDARY,"Secondary"}, 
    {PROGTOLREFTYPE_TERTIARY, "Tertiary"},       
    {-1,                   TABLE_TERM}    
};

static Type_to_str progtol_projzone_tbl[]={
    {PROGTOLPROJZONE_NONE,       "None"},
    {PROGTOLPROJZONE_BELOWGTOL,  "Below"},
    {PROGTOLPROJZONE_INSIDEGTOL, "Inside"},
    {-1,                   TABLE_TERM}        
};

/*====================================================================*\
    FUNCTION :	ProUtilTypeStr()
    PURPOSE  :	General translation from a type to a string
\*====================================================================*/
int ProUtilTypeStr(
    Type_to_str table[],
    int type,
    char *type_name)
{
    int i;

/*--------------------------------------------------------------------*\
    Search for the type in the array of types.
\*--------------------------------------------------------------------*/
    for(i=0; strcmp(table[i].str, TABLE_TERM); i++)
    {
        if(table[i].type == type)
        {
	    ProUtilstrcpy(type_name, (const char *)table[i].str);
            return(0);
        }
    }

/*--------------------------------------------------------------------*\
    If we're still here we did not find it, so return "Unknown"
\*--------------------------------------------------------------------*/
    ProTKSprintf(type_name, "**UNKNOWN : %d **", type);
    return(0);
}

/*====================================================================*\
    FUNCTION :	ProUtilFeattypeStr()
    PURPOSE  :	Provide a feature type name given an integer type.
\*====================================================================*/
int ProUtilFeattypeStr(
    ProFeattype type,
    char *type_name)
{
    return(ProUtilTypeStr(feattype_table, type, type_name));
}

/*====================================================================*\
    FUNCTION :	ProUtilObjtypeStr()
    PURPOSE  :	Provide an object type name given an integer type.
\*====================================================================*/
int ProUtilObjtypeStr(
    ProType type,
    char *type_name)
{
    return(ProUtilTypeStr(objtype_table, type, type_name));
}

/*====================================================================*\
    FUNCTION :	ProUtilSrftypeStr()
    PURPOSE  :	Provide an surface type name given an integer type.
\*====================================================================*/
int ProUtilSrftypeStr(
    ProSrftype type,
    char *type_name)
{
    return(ProUtilTypeStr(surftype_table, type, type_name));
}

/*====================================================================*\
    FUNCTION :	ProUtilEnttypeStr()
    PURPOSE  :	Provide an entity (edge or curve) type name given an
			integer type.
\*====================================================================*/
int ProUtilEnttypeStr(
    ProEnttype type,
    char *type_name)
{
    return(ProUtilTypeStr(enttype_table, type, type_name));
}

/*====================================================================*\
    FUNCTION :	ProUtilEnttypeStr()
    PURPOSE  :	Provide an entity (edge or curve) type name given an
			integer type.
\*====================================================================*/
int ProUtilMfgtypeStr(
    ProMfgType type,
    char *type_name)
{
    return(ProUtilTypeStr(mfgtype_table, type, type_name));
}

/*====================================================================*\
    FUNCTION :	ProUtilExtRefScopeStr()
    PURPOSE  :	Provide an external references allow  name given an
		integer value.
\*====================================================================*/
int ProUtilExtRefScopeStr(
    ProExtRefScope type,
    char *type_name)
{
    return(ProUtilTypeStr(proextrefscore_table, type, type_name));
}

/*====================================================================*\
    FUNCTION :	ProUtilGtolTypeStr()
    PURPOSE  :	
\*====================================================================*/
int ProUtilGtolTypeStr(
    ProGtolType type,
    char *type_name)
{
    return(ProUtilTypeStr(progtol_tbl, type, type_name));
}

/*====================================================================*\
    FUNCTION :	ProUtilGtolMaterialCondStr()
    PURPOSE  :	
\*====================================================================*/
int ProUtilGtolMaterialCondStr(
    ProGtolMaterialCond type,
    char *type_name)
{
    return(ProUtilTypeStr(progtol_materialcond_tbl, type, type_name));
}

/*====================================================================*\
    FUNCTION :	ProUtilLeaderTypeStr()
    PURPOSE  :	
\*====================================================================*/
int ProUtilLeaderTypeStr(
    ProLeaderType type,
    char *type_name)
{
    return(ProUtilTypeStr(progtol_leader_tbl, type, type_name));
}

/*====================================================================*\
    FUNCTION :	ProUtilGtolPlacementTypeStr()
    PURPOSE  :	
\*====================================================================*/
int ProUtilGtolPlacementTypeStr(
    ProGtolPlacementType type,
    char *type_name)
{
    return(ProUtilTypeStr(progtol_placement_tbl, type, type_name));
}

/*====================================================================*\
    FUNCTION :	ProUtilGtolRefItemTypeStr()
    PURPOSE  :	
\*====================================================================*/
int ProUtilGtolRefItemTypeStr(
    ProGtolRefItemType type,
    char *type_name)
{
    return(ProUtilTypeStr(progtol_refitem_tbl, type, type_name));
}

/*====================================================================*\
    FUNCTION :	ProUtilGtolReferenceTypeStr()
    PURPOSE  :	
\*====================================================================*/
int ProUtilGtolReferenceTypeStr(
    ProGtolReferenceType type,
    char *type_name)
{
    return(ProUtilTypeStr(progtol_reftype_tbl, type, type_name));
}

/*====================================================================*\
    FUNCTION :	ProUtilGtolProjzoneStr()
    PURPOSE  :	
\*====================================================================*/
int ProUtilGtolProjzoneStr(
    ProGtolProjzone type,
    char *type_name)
{
    return(ProUtilTypeStr(progtol_projzone_tbl, type, type_name));
}
