

/*--------------------------------------------------------------------*\
Pro/TOOLKIT includes
\*--------------------------------------------------------------------*/
#include <ProToolkit.h>
#include <ProObjects.h>
#include <ProSurface.h>
#include <ProFeature.h>
#include <ProFeatType.h>
#include <ProSelection.h>
#include <ProSolid.h>
#include <ProAsmcomp.h>
#include <ProEdge.h>
#include <ProAxis.h>
#include <ProCsys.h>
#include <ProCurve.h>
#include <ProPoint.h>
#include <ProGeomitem.h>
#include <ProNcseq.h>
#include "UtilString.h"
/*--------------------------------------------------------------------*\
Application includes
\*--------------------------------------------------------------------*/
#include "TestError.h"
#include "UtilMath.h"
#include "UtilVisit.h"
#include "UtilCollect.h"

/*--------------------------------------------------------------------*\
Application data
\*--------------------------------------------------------------------*/

typedef struct 
{
    ProAppData  app_data;       /* The caller's app_data */
    ProFunction action;         /* The caller's action function */
} AsmComp_data_t ;

/*--------------------------------------------------------------------*\
    We need our own app_data to pass to ProUtilAsmCompVisit() which
    contains both the app_data and the action function pointer input to
    this function, plus our own context data so that OUR action function
    has all it needs to call the user's action function.
\*--------------------------------------------------------------------*/

typedef ProError (*ProTestAsmTravAction) (ProAsmcomppath*, ProAppData);

typedef struct 
{
    ProAppData     app_data;	/* The caller's app_data */
    ProTestAsmTravAction    action;	/* The caller's action function */
    ProAsmcomppath comp_path;	/* The current component path */
} AsmTrav_data_t ;

/*====================================================================*\
    FUNCTION :	ProUtilAsmcompVisit()
    PURPOSE  :	Visit function for components of an assembly
\*====================================================================*/
ProError ProUtilAsmcompVisit(
    ProAssembly assembly,
    ProFunction action,
    ProAppData  app_data )
{
    ProError status;

/*--------------------------------------------------------------------*\
    Visit all the features, using a filter to see only assembly components
\*--------------------------------------------------------------------*/
    status = ProSolidFeatVisit(assembly, (ProFeatureVisitAction)action,
		(ProFeatureFilterAction)ProUtilAsmcompFilterAction, app_data);
    TEST_CALL_REPORT("ProSolidFeatVisit()", "ProUtilAsmcompVisit()",
                                status, status != PRO_TK_NO_ERROR);
    
    return(status);
}

/*====================================================================*\
    FUNCTION :	ProUtilFeatvisVisit()
    PURPOSE  :	Visit function for visible features
\*====================================================================*/
ProError ProUtilFeatvisVisit(
    ProSolid	solid,
    ProFunction action,
    ProAppData  app_data )
{
    ProError status;

/*--------------------------------------------------------------------*\
    Visit all the features, using a filter to see only assembly components
\*--------------------------------------------------------------------*/
    status = ProSolidFeatVisit(solid, (ProFeatureVisitAction)action,
				(ProFeatureFilterAction)ProUtilFeatvisFilterAction, app_data);
    TEST_CALL_REPORT("ProSolidFeatVisit()", "ProUtilFeatvisVisit()",
                                status, status != PRO_TK_NO_ERROR);
    
    return(status);
}

/*====================================================================*\
    FUNCTION :	ProUtilGeomitemactiveVisit()
    PURPOSE  :	Visit function for active geomitems
\*====================================================================*/
ProError ProUtilGeomitemactiveVisit( 
    ProFeature *p_feature,
    ProType item_type,
    ProFunction action,
    ProAppData  app_data)
{
    ProError status;

/*--------------------------------------------------------------------*\
    Visit all the features, using a filter to see only assembly components
\*--------------------------------------------------------------------*/
    status = ProFeatureGeomitemVisit(p_feature, item_type,
	    (ProGeomitemAction)action,
	     (ProGeomitemFilter)ProUtilGeomitemactiveFilterAction, app_data);
    TEST_CALL_REPORT("ProFeatureGeomitemVisit()", 
	"ProUtilGeomitemactiveVisit()",
        status, status != PRO_TK_NO_ERROR);
   
    return status; 
}


/*====================================================================*\
    FUNCTION :	ProUtilAsmTravAction()
    PURPOSE  :	Action function to be called for features inside
		ProUtilAsmTraverse() to recursively visit features which
		are assemblt components.
\*====================================================================*/
ProError ProUtilAsmTravAction(
    ProFeature *component,
    ProError instatus,
    ProAppData app_data )
{
    ProError status;
    ProSolid model;
    AsmTrav_data_t *asm_data = (AsmTrav_data_t*)app_data;
    ProMdlType model_type;

/*--------------------------------------------------------------------*\
    Increment the component path
\*--------------------------------------------------------------------*/
    asm_data->comp_path.comp_id_table[asm_data->comp_path.table_num++] =
		component->id;
    asm_data->comp_path.comp_id_table[asm_data->comp_path.table_num] = -1;
    
/*--------------------------------------------------------------------*\
    Get the ProMdl for this assembly component
\*--------------------------------------------------------------------*/
    status = ProAsmcompMdlGet(component, (ProMdl *) &model);
    TEST_CALL_REPORT("ProAsmcompMdlGet()", "ProUtilAsmTravAction()",
                                status, status != PRO_TK_NO_ERROR);

/*--------------------------------------------------------------------*\
    Call the action function for this component, with the user's app_data
\*--------------------------------------------------------------------*/
    status = (*asm_data->action)
        (&asm_data->comp_path, asm_data->app_data);

/*--------------------------------------------------------------------*\
    If the model is an assembly, visit its components recursively
\*--------------------------------------------------------------------*/

    ProMdlTypeGet(model, &model_type);
    TEST_CALL_REPORT("ProMdlTypeGet()", "ProUtilAsmTravAction()",
                                status, status != PRO_TK_NO_ERROR);

    if(model_type == PRO_MDL_ASSEMBLY)
	ProUtilAsmcompVisit((ProAssembly)model,
			    (ProFunction)ProUtilAsmTravAction,app_data);

/*--------------------------------------------------------------------*\
    Decrement the component path
\*--------------------------------------------------------------------*/
    asm_data->comp_path.comp_id_table[asm_data->comp_path.table_num--] = -1;

    return(PRO_TK_NO_ERROR);
}

/*====================================================================*\
    FUNCTION :	ProUtilAsmTraverse()
    PURPOSE  :	Visit function for components of an assembly at ALL levels
\*====================================================================*/
ProError ProUtilAsmTraverse(
    ProAssembly assembly,
    ProFunction action,
    ProAppData app_data)
{
    ProError status;
    AsmTrav_data_t asm_data;
    ProIdTable comp_id_table;

/*--------------------------------------------------------------------*\
    Initialize our asm_data
\*--------------------------------------------------------------------*/
    asm_data.app_data = app_data;
    asm_data.action = (ProTestAsmTravAction)action;
    comp_id_table[0] = -1;
    status = ProAsmcomppathInit(assembly, comp_id_table, 0, 
                    &asm_data.comp_path);
    TEST_CALL_REPORT("ProAsmcomppathInit()", "ProUtilAsmTraverse()",
				status, status != PRO_TK_NO_ERROR);

/*--------------------------------------------------------------------*\
    Visit the components are this level
\*--------------------------------------------------------------------*/
    ProUtilAsmcompVisit(assembly, (ProFunction)ProUtilAsmTravAction,
						(ProAppData)&asm_data);

    return(PRO_TK_NO_ERROR);
}

/*====================================================================*\
    FUNCTION :  ProUtilAsmFeatsTraverse()
    PURPOSE  :  Visit function for features of an assembly at *ALL* levels
\*====================================================================*/
ProError ProUtilAsmFeatsTraverse(
    ProAssembly assembly,
    ProFunction action,
    ProAppData app_data)
{
    ProError status;
    AsmTrav_data_t asm_data;
    ProIdTable comp_id_table;
 
/*--------------------------------------------------------------------*\
    Initialize our asm_data
\*--------------------------------------------------------------------*/
    asm_data.app_data = app_data;
    asm_data.action = (ProTestAsmTravAction) action;
    comp_id_table[0] = -1;
    status = ProAsmcomppathInit(assembly, comp_id_table, 0,
                    &asm_data.comp_path);
    TEST_CALL_REPORT("ProAsmcomppathInit()", "ProUtilAsmTraverse()",
                                status, status != PRO_TK_NO_ERROR);
 
/*--------------------------------------------------------------------*\
    Visit the components are this level
\*--------------------------------------------------------------------*/
    ProUtilAsmcompVisit(assembly, (ProFunction) ProUtilAsmTravAction,
                                                (ProAppData)&asm_data);
 
    return(PRO_TK_NO_ERROR);
}


/*====================================================================*\
    FUNCTION :	ProUtilAsmcompFilterAction()
    PURPOSE  :	A filter used by ProUtilAsmCompVisit() to visit
		features which are assembly components
\*====================================================================*/
ProError ProUtilAsmcompFilterAction(
    ProFeature *feature,
    ProAppData app_data)
{
    ProError status;
    ProFeattype ftype;

/*--------------------------------------------------------------------*\
    Get the feature type
\*--------------------------------------------------------------------*/
    status = ProFeatureTypeGet(feature, &ftype);

/*--------------------------------------------------------------------*\
    If the feature is an assembly component,
	return NO ERROR,
    else
	return CONTINUE
\*--------------------------------------------------------------------*/
    if(ftype == PRO_FEAT_COMPONENT)
	return(PRO_TK_NO_ERROR);
    return(PRO_TK_CONTINUE);
}

/*=========================================================================*\
    Function:	ProUtilFeatvisFilterAction()
    Purpose:	Filter function for ProSolidFeatVisit, allow to visit only 
		visible features
    Returns:	PRO_TK_NO_ERROR - feature visible
		PRO_TK_CONTINUE - otherwise
\*=========================================================================*/
ProError ProUtilFeatvisFilterAction(
    ProFeature *p_feature,	/* In : the feature */
    ProAppData app_data)	/* In : appdata (not used) */
{
    ProError status;
    ProBoolean visible;

    status = ProFeatureVisibilityGet(p_feature, &visible);
    TEST_CALL_REPORT("ProFeatureVisibilityGet()", 
				    "ProUtilFeatvisFilterAction()", 
				    status, status != PRO_TK_NO_ERROR);
    if (status == PRO_TK_NO_ERROR)
	status = (visible == PRO_B_TRUE ? PRO_TK_NO_ERROR : PRO_TK_CONTINUE);

    return (status);
}


/*=========================================================================*\
    Function:	ProUtilByNameFilterAction()
    Purpose:	Allow to visit only modelitem with a specified name
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_CONTINUE - no valid modelitem found
\*=========================================================================*/
ProError ProUtilByNameFilterAction(
    ProModelitem *p_modelitem,	    /* In : The modelitem */
    FindByName   *p_findbyname)	    /* In : The find structure */
{
    ProName  name;
    ProError status;

    status = ProModelitemNameGet(p_modelitem, name);
    TEST_CALL_REPORT("ProModelitemNameGet()", 
				    "ProUtilFindByNameFilterAction()", 
	    status, status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
    if (status == PRO_TK_NO_ERROR) 
    {
	if (ProUtilWstrCmp(p_findbyname->name, name)==0)
	    status = PRO_TK_NO_ERROR;
	else 
	    status = PRO_TK_CONTINUE;
    }
    else if (status == PRO_TK_E_NOT_FOUND)
	status = PRO_TK_CONTINUE;

    return (status);
}

/*=========================================================================*\
    Function:	ProUtilFeatvisByNameFilterAction()
    Purpose:	Allow to visit only visible features with a specified name
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_CONTINUE - no valid feature found found
\*=========================================================================*/
ProError ProUtilFeatvisByNameFilterAction(
    ProFeature   *p_feature,	    /* In : The feature */
    FindByName   *p_findbyname)	    /* In : The find structure */
{
    ProError  status;

    status = ProUtilFeatvisFilterAction(p_feature, NULL);
    if (status == PRO_TK_NO_ERROR)
	status = ProUtilByNameFilterAction((ProModelitem*)p_feature, 
	    p_findbyname);
	
    return (status);
}

/*=========================================================================*\
    Function:	ProUtilGeomitemactiveFilterAction()
    Purpose:	Filter function for ProFeatureGeomitemVisit, allow to visit 
		only active geomitems
    Returns:	PRO_TK_NO_ERROR - feature visible
		PRO_TK_CONTINUE - otherwise
\*=========================================================================*/
ProError ProUtilGeomitemactiveFilterAction(
    ProGeomitem *p_geomitem,	/* In : the geomitem */
    ProAppData app_data)	/* In : appdata (not used) */
{
    ProBoolean test;
    ProError status;

    TEST_CALL_REPORT( "ProGeomitemFilter", "ProUtilGeomitemactiveFilterAction",
	PRO_TK_NO_ERROR, 0 );
    status = ProGeomitemIsInactive(p_geomitem, &test);
    TEST_CALL_REPORT("ProGeomitemIsInactive()", 
	"ProUtilGeomitemactiveFilterAction()", 
        status, status != PRO_TK_NO_ERROR);
    return (test != PRO_B_TRUE ? PRO_TK_NO_ERROR : PRO_TK_CONTINUE);
}


/*=========================================================================*\
    Function:   ProUtilAxisByVectorFilterAction()
    Purpose:    Allow to visit only axis which parallel (perpendicular)
                to a vector
    Returns:    PRO_TK_NO_ERROR - success;
                PRO_TK_CONTINUE - no valid axis found
\*=========================================================================*/
ProError ProUtilAxisByVectorFilterAction(
    ProAxis axis,                               /* In : The Axis */
    CollectAxisByVector *p_find_by_vector)      /* In : find data */
{
    ProGeomitemdata  *p_gi_data;
    Pro3dPnt         vec;
    double           dot;
    ProError         status = PRO_TK_CONTINUE;


    TEST_CALL_REPORT( "ProAxisFilterAction", "ProUtilAxisByVectorFilterAction",
	PRO_TK_NO_ERROR, 0 );

    status = ProAxisDataGet(axis, &p_gi_data);
    TEST_CALL_REPORT("ProAxisDataGet()", "ProUtilAxisByVectorFilterAction()",
                                    status, status != PRO_TK_NO_ERROR);
    ProUtilVectorDiff(p_gi_data->data.p_curve_data->line.end2,
                      p_gi_data->data.p_curve_data->line.end1, vec);
    status = ProGeomitemdataFree(&p_gi_data);
    TEST_CALL_REPORT("ProGeomitemdataFree()",
        "ProUtilAxisByVectorFilterAction()", status, status != PRO_TK_NO_ERROR);
    ProUtilVectorNormalize(vec, vec);
    dot = fabs(ProUtilVectorDot(vec, p_find_by_vector->vec));
    if ((p_find_by_vector->option==USER_PARALLEL && 1-dot<EPSM6) ||
         (p_find_by_vector->option==USER_PERPENDICULAR && dot<EPSM6))
         status = PRO_TK_NO_ERROR;
    else
         status = PRO_TK_CONTINUE;
    return (status);
}

/*=========================================================================*\
    Function:	ProUtilCollectDtmCurveFeatFilter()
    Purpose:	Allow to visit only dtm curve features
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_CONTINUE - no valid feature found found
\*=========================================================================*/
ProError ProUtilCollectDtmCurveFeatFilter(
    ProFeature   *p_feature,	    /* In : The feature */
    ProAppData   app_data)	    /* In : The find structure */
{
    ProError  status;
    ProFeattype ftype;
    
    TEST_CALL_REPORT( "ProFeatureFilterAction", 
        "ProUtilCollectDtmCurveFeatFilter",
	PRO_TK_NO_ERROR, 0 );

    status = ProFeatureTypeGet(p_feature, &ftype);
    TEST_CALL_REPORT("ProFeatureTypeGet()", "ProUtilCollectDtmCurveFeatFilter()",
			    status, status != PRO_TK_NO_ERROR);

    return(ftype == PRO_FEAT_CURVE ? PRO_TK_NO_ERROR : PRO_TK_CONTINUE);
}

/*====================================================================*\
    FUNCTION :	ProUtilCollectDtmPointFeatFilter()
    PURPOSE  :	A filter used to visit only Datum Points features
\*====================================================================*/
ProError ProUtilCollectDtmPointFeatFilter(
    ProFeature *feature,
    ProAppData app_data)
{
    ProError status;
    ProFeattype ftype;

/*--------------------------------------------------------------------*\
    Get the feature type
\*--------------------------------------------------------------------*/
    status = ProFeatureTypeGet(feature, &ftype);

/*--------------------------------------------------------------------*\
    If the feature is Datum Point,
	return NO ERROR,
    else
	return CONTINUE
\*--------------------------------------------------------------------*/
    if(ftype == PRO_FEAT_DATUM_POINT)
	return(PRO_TK_NO_ERROR);
    return(PRO_TK_CONTINUE);
}


/*=========================================================================*\
    Function:	ProUtilCollectDtmCurveFeatFilter()
    Purpose:	Allow to visit only dtm curve features
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_CONTINUE - no valid feature found found
\*=========================================================================*/
ProError ProUtilCollectNCSeqFeatFilter(
    ProFeature   *p_feat,	    /* In : The feature */
    ProAppData   tmp_p_nc_seq)	    /* In : The find structure */
{
    ProError status;
    ProBoolean is_nc_seq;
    
    TEST_CALL_REPORT( "ProFeatureFilterAction", "ProUtilCollectNCSeqFeatFilter",
	PRO_TK_NO_ERROR, 0 );

    status = ProFeatureIsNcseq(p_feat, &is_nc_seq);
    TEST_CALL_REPORT("ProFeatureIsNcseq()",
                        "ProTestFeatFilterNcseq()", status,
                        (status != PRO_TK_NO_ERROR));

    return (is_nc_seq == PRO_B_TRUE? PRO_TK_NO_ERROR : PRO_TK_CONTINUE);

}

/*=========================================================================*\
    Function:	ProUtilGeomitemByNameFilterAction()
    Purpose:	Allow to visit only geomitem with a specified name
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_CONTINUE - no valid modelitem found
\*=========================================================================*/
ProError ProUtilGeomitemByNameFilterAction(
    ProAppData geomitem,	    /* In : The geomitem */
    FindByName   *p_findbyname)	    /* In : The find structure */
{
    ProName  name;
    ProError status;
    ProModelitem modelitem = p_findbyname->p_modelitem[0];

    switch (modelitem.type)
    {
	case PRO_CSYS:
	    status = ProCsysToGeomitem((ProSolid)modelitem.owner, 
		(ProCsys)geomitem, &modelitem);
	    TEST_CALL_REPORT("ProCsysToGeomitem()", 
		"ProUtilGeomitemByNameFilterAction()", 
		status, status != PRO_TK_NO_ERROR);
	    break;
	case PRO_AXIS:
	    status = ProAxisToGeomitem((ProSolid)modelitem.owner, 
		(ProAxis)geomitem, &modelitem);
	    TEST_CALL_REPORT("ProAxisToGeomitem()", 
		"ProUtilGeomitemByNameFilterAction()", 
		status, status != PRO_TK_NO_ERROR);
	    break;
	case PRO_SURFACE:
	    status = ProSurfaceToGeomitem((ProSolid)modelitem.owner, 
		(ProSurface)geomitem, &modelitem);
	    TEST_CALL_REPORT("ProSurfaceToGeomitem()", 
		"ProUtilGeomitemByNameFilterAction()", 
		status, status != PRO_TK_NO_ERROR);
	    break;
	default:
	    status = PRO_TK_GENERAL_ERROR;
    }
    
    if (status == PRO_TK_NO_ERROR)
    {
	status = ProModelitemNameGet(&modelitem, name);
	TEST_CALL_REPORT("ProModelitemNameGet()", 
		"ProUtilGeomitemByNameFilterAction()", status,
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
    }
    if (status == PRO_TK_NO_ERROR) 
    {
	if (ProUtilWstrCmp(p_findbyname->name, name)==0)
	    status = PRO_TK_NO_ERROR;
	else 
	    status = PRO_TK_CONTINUE;
    }
    else if (status == PRO_TK_E_NOT_FOUND)
	status = PRO_TK_CONTINUE;

    return (status);
}
