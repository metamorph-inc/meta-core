

#ifndef PROUTILCOLLECTION_H
#define PROUTILCOLLECTION_H


/*----------------------------------------------------------*\
    Headers
\*----------------------------------------------------------*/
#include <ProToolkit.h>
#include <ProAnimate.h>
#include <ProAsmcomp.h>
#include <ProContour.h>
#include <ProCurve.h>
#include <ProExtobj.h>
#include <ProExtobjRef.h>
#include <ProModelitem.h>
#include <ProSolid.h>
#include <ProMfg.h>
#include <ProNote.h>
#include <ProParameter.h>
#include <ProQuilt.h>
#include <ProSimprep.h>
#include <ProSurface.h>
#include <ProLayer.h>
#include <ProFamtable.h>
#include <ProFaminstance.h>
#include <ProRelSet.h>
#include <ProXsec.h>
#include "UtilVisit.h"

/*----------------------------------------------------------*\
    Macros
\*----------------------------------------------------------*/
#define USER_PARALLEL 0		    /* To find parallel vectors */
#define USER_PERPENDICULAR 1	    /* To find prependicuilar vectors */


/*----------------------------------------------------------*\
    Data types
\*----------------------------------------------------------*/
typedef struct tag_CurveComponent
    {
	ProCurve    p_owner;/* The composite curve with component */
	ProCurve    p_curve;/* The curve component being visited */
	int	    index;  /* The index of the component in the curve*/
	ProBoolean  flip;   /* This is PRO_TK_TRUE if the component is flipped*/
    } CurveComponent;


typedef struct tag_ElemtreeElement
    {
	ProElement  p_element;	/* The handle to the element */
	ProElempath p_elempath;	/* The handle to element path */
    } ElemtreeElement;


typedef struct tag_ExtobjReference
    {
	ProExtobj	extobj;		/* The handle to the external object */
	ProWExtobjRef	p_extobj_ref;	/* The handle to the reference */
	ProExtobjrefStatus ref_status;  /* The reference status */ 
    } ExtobjReference;

/*=========================================================================*\
    Function:	ProUtilCollectAnimFrames()
    Purpose:	Return a list of frames conteained in the animation movie
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectAnimFrames( 
    ProAnimMovie    anim_movie,	    /* In:  The handle to the animation 
					    movie */
    ProAnimFrame    **p_frames	    /* Out: ProArray with collected frames. 
					    The function allocates memory 
					    for this argument, but you must 
					    free it. To free the memory, 
					    call the function ProArrayFree()*/
);

/*=========================================================================*\
    Function:	ProUtilCollectAnimObjects()
    Purpose:	Return a list of objects conteained in the animation frame
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectAnimObjects( 
    ProAnimFrame    anim_frame,	    /* In:  The handle to the animation 
					    frame */
    ProAnimObj	    **p_objects	    /* Out: ProArray with collected objects. 
					    The function allocates memory 
					    for this argument, but you must 
					    free it. To free the memory, 
					    call the function ProArrayFree()*/
);
/*=========================================================================*\
    Function:	ProUtilCollectParameters()
    Purpose:	Return a list of parameters which belong to the model item
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectParameters( 
    ProModelitem    *p_modelitem,   /* In:  The model item */
    ProParameter    **p_parameters  /* Out: ProArray with collected parameters.
					    The function allocates 
					    memory for this argument, but 
					    you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
);

/*=========================================================================*\
    Function:	ProUtilCollectModelNotes()
    Purpose:	Return a list of notes which belong to the model
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectModelNotes( 
    ProMdl	    p_model,	    /* In:  The handle to the model */
    ProModelitem    **p_notes	    /* Out: ProArray with collected notes.
					    The function allocates 
					    memory for this argument, but 
					    you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
);

/*=========================================================================*\
    Function:	ProUtilCollectMfgTools()
    Purpose:	Return a list of tools in the manufacturing model
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectMfgTools( 
    ProMfg	    p_mfg,	    /* In:  The manufacturing model */
    ProTool	    **p_tools	    /* Out: ProArray with collected tools. 
					    The function allocates 
					    memory for this argument, but 
					    you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
);



/*=========================================================================*\
    Function:	ProUtilCollectFeatureGeomitems()
    Purpose:	Return a list of geometry items created by the feature
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectFeatureGeomitems( 
    ProFeature	    *p_feature,	    /* In:  The feature handle */
    ProType	    item_type,	    /* In:  The type of expected geometry item */
    ProGeomitem	    **p_geomitems   /* Out: ProArray with collected geometry 
					    items. The function allocates 
					    memory for this argument, but 
					    you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
);


/*=========================================================================*\
    Function:	ProUtilCollectExtobjReferences()
    Purpose:	Return a list of external objects references
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectExtobjReferences( 
    ProExtobj	    *p_extobj,	    /* In:  The handle to the external object */
    ExtobjReference **p_extobj_refs	    /* Out: ProArray with collected external 
					    object references. The function 
					    allocates memory for this argument, 
					    but you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
);


/*=========================================================================*\
    Function:	ProUtilCollectExtobj()
    Purpose:	Return a list of external objects which belong to the model
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectExtobj( 
    ProMdl	    p_model,	    /* In:  The handle to the solid */
    ProExtobjClass  *p_extobj_class,/* In:  The external object class */
    ProExtobj	    **p_extobjs	    /* Out: ProArray with collected external 
					    objects. The function allocates 
					    memory for this argument, but 
					    you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
);



/*=========================================================================*\
    Function:	ProUtilCollectElemtreeElements()
    Purpose:	Return a list of the subelements of the specified element
		in the element tree. 
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectElemtreeElements(
    ProElement	    p_elemtree,	/* In:	The element tree */
    ProElempath	    p_elempath,	/* In:	The path to the element */
    ElemtreeElement **p_elements    /* Out: ProArray with collected elements. 
					    The function allocates memory 
					    for this argument, but you must 
					    free it. To free the memory, 
					    call the function 
					    ProUtilElemtreeElementArrayFree*/
);

/*=========================================================================*\
    Function:	ProUtilElemtreeElementArrayFree()
    Purpose:	Free memory allocated by ProUtilCollectElemtreeElements
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilElemtreeElementArrayFree(
    ElemtreeElement **p_elements);

/*=========================================================================*\
    Function:	ProUtilCollectCurveComponents()
    Purpose:	Return a list the components of the specified composite curve. 
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectCurveComponents(
    ProCurve	    p_curve,	/* In:  The composite curve being processed */
    CurveComponent  **p_components  /* Out: ProArray with collected components. 
					    The function allocates memory 
					    for this argument, but you must 
					    free it. To free the memory, 
					    call the function ProArrayFree()*/
);

/*=========================================================================*\
    Function:	ProUtilCollectContourEdges()
    Purpose:	Return a list of edges which belong to the contour
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectContourEdges( 
    ProSurface	    p_surface,	    /* In:  The surface of the contour */
    ProContour	    p_contour,	    /* In:  The handle to the contour */
    ProEdge	    **p_edges	    /* Out: ProArray with collected edges . 
					    The function allocates memory 
					    for this argument, but you must 
					    free it. To free the memory, 
					    call the function ProArrayFree()*/
);


/*=========================================================================*\
    Function:	ProUtilCollectSolidCsys()
    Purpose:	Return a list of csys which belong to the solid
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectSolidCsys( 
    ProSolid	    p_solid,	    /* In:  The handle to the solid */
    ProCsys	    **p_csys	    /* Out: ProArray with collected csys . 
					    The function allocates memory 
					    for this argument, but you must 
					    free it. To free the memory, 
					    call the function ProArrayFree()*/
);


/*=========================================================================*\
    Function:	ProUtilCollectSolidAxis()
    Purpose:	Return a list of axis which belong to the solid
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectSolidAxis( 
    ProSolid	    p_solid,	    /* In:  The handle to the solid */
    ProAxis	    **p_axis	    /* Out: ProArray with collected axis . 
					    The function allocates memory 
					    for this argument, but you must 
					    free it. To free the memory, 
					    call the function ProArrayFree()*/
);


/*=========================================================================*\
    Function:	ProUtilCollectSolidQuilts()
    Purpose:	Return a list of quilts which belong to the solid
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectSolidQuilts( 
    ProSolid	    p_solid,	    /* In:  The handle to the solid */
    ProQuilt	    **p_quilts	    /* Out: ProArray with collected quilts . 
					    The function allocates memory 
					    for this argument, but you must 
					    free it. To free the memory, 
					    call the function ProArrayFree()*/
);


/*=========================================================================*\
    Function:	ProUtilCollectSolidSurfaces()
    Purpose:	Return a list of surfaces which belong to the solid
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectSolidSurfaces( 
    ProSolid	    p_solid,	    /* In:  The handle to the solid */
    ProSurface	    **p_surfaces    /* Out: ProArray with collected surfaces. 
					    The function allocates memory 
					    for this argument, but you must 
					    free it. To free the memory, 
					    call the function ProArrayFree()*/
);


/*=========================================================================*\
    Function:	ProUtilCollectQuiltSurfaces()
    Purpose:	Return a list of surfaces which belong to the quilt
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectQuiltSurfaces( 
    ProQuilt	    p_quilt,	    /* In:  The handle to the quilt */
    ProSurface	    **p_surfaces    /* Out: ProArray with collected surfaces. 
					    The function allocates memory 
					    for this argument, but you must 
					    free it. To free the memory, 
					    call the function ProArrayFree()*/
);


/*=========================================================================*\
    Function:	ProUtilCollectSolidSimpreps()
    Purpose:	Return a list of simplified representations in the solid
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectSolidSimpreps( 
    ProSolid	    p_solid,	    /* In:  The solid handle */
    ProSimprep	    **p_simpreps    /* Out: ProArray with collected simp reps
					    items. The function allocates 
					    memory for this argument, but 
					    you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
);

/*=========================================================================*\
    Function:	ProUtilCollectSolidFeatures()
    Purpose:	Return a list of features in the solid
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectSolidFeatures( 
    ProSolid	    p_solid,	    /* In:  The solid handle */
    ProFeature	    **p_features    /* Out: ProArray with collected features
					    items. The function allocates 
					    memory for this argument, but 
					    you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
);

/*=========================================================================*\
    Function:	ProUtilFeatvisFilterAction()
    Purpose:	Filter function for ProSolidFeatVisit, allow to visit only 
		visible features
    Returns:	PRO_TK_NO_ERROR - feature visible
		PRO_TK_CONTINUE - otherwise
\*=========================================================================*/
ProError ProUtilFeatvisFilterAction(
    ProFeature *p_feature,	/* In : the feature */
    ProAppData app_data);	/* In : appdata (not used) */


/*=========================================================================*\
    Function:	ProUtilCollectSolidFeatvis()
    Purpose:	Return a list of visible features in the solid
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectSolidFeatvis( 
    ProSolid	    p_solid,	    /* In:  The solid handle */
    ProFeature	    **p_features    /* Out: ProArray with collected features
					    items. The function allocates 
					    memory for this argument, but 
					    you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
);

/*=========================================================================*\
    Function:	ProUtilAsmcompFilterAction()
    Purpose:	Filter function for ProSolidFeatVisit, allow to visit only 
		assembly components
    Returns:	PRO_TK_NO_ERROR - feature is assembly component
		PRO_TK_CONTINUE - otherwise
\*=========================================================================*/
ProError ProUtilAsmcompFilterAction(
    ProFeature *p_feature,	    /* In : the feature */
    ProAppData app_data);	    /* In : appdata (not used) */


/*=========================================================================*\
    Function:	ProUtilCollectAsmcomp()
    Purpose:	Return a list of visible features in the solid
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectAsmcomp( 
    ProAssembly	    p_assy,	    /* In:  The assembly handle */
    ProAsmcomp	    **p_asmcomps    /* Out: ProArray with collected asm 
					    comps. The function allocates 
					    memory for this argument, but 
					    you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
);


/*=========================================================================*\
    Function:	ProUtilCollectSurfaceContours()
    Purpose:	Return a list of contours which belong to the surface
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectSurfaceContours( 
    ProSurface	    p_surfaces,	    /* In:  The handle to the quilt */
    ProContour	    **p_contours    /* Out: ProArray with collected contours. 
					    The function allocates memory 
					    for this argument, but you must 
					    free it. To free the memory, 
					    call the function ProArrayFree()*/
);

/*=========================================================================*\
    Function:	ProUtilCollectMdlLayers()
    Purpose:	Return a list of layers in the model 
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectMdlLayers( 
    ProMdl	model,	    /* In:  The handle to the model */
    ProLayer	**p_layer   /* Out: ProArray with collected layers
				    The function allocates memory 
				    for this argument, but you must 
				    free it. To free the memory, 
				    call the function ProArrayFree()*/
);

/*=========================================================================*\
    Function:	ProUtilCollectDimVisitAction()
    Purpose:	Add any dimension given by pointer to the array of dimensions
    Returns:	PRO_TK_NO_ERROR - success;
\*=========================================================================*/
ProError ProUtilCollectDimVisitAction (ProDimension *dimension,
                                       ProError     filter_status, /* ignored */
                                       ProAppData   data
);
/*=========================================================================*\
    Function:	ProUtilCollectDimensions()
    Purpose:	Return a list of dimensions in the part, assembly, or drawing
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters*/

/*=========================================================================*/
ProError ProUtilCollectDimension( 
    ProMdl	model,	    /* In:  The handle to the model */
    ProBoolean   refdim,    /* In:  PRO_B_TRUE for collecting reference dimensions,
                                    PRO_B_FALSE for collecting standard dimensions */ 
    ProDimension **p_dims   /* Out: ProArray with collected dimensions. 
				    The function allocates memory 
				    for this argument, but you must 
				    free it. To free the memory, 
				    call the function ProArrayFree()*/
);

/*=========================================================================*\
    Function:	ProUtilFindFeatureByName()
    Purpose:	Return a feature with specified name
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_E_NOT_FOUND - no valid feature found
\*=========================================================================*/
ProError ProUtilFindFeatureByName(
    ProSolid p_solid,		    /* In : The solid */
    ProName  name,		    /* In : The feature name */ 
    ProFeature *p_feature);	    /* Out: Founded feature,user's memory */


/*=========================================================================*\
    Function:	ProUtilFindFeatvisByName()
    Purpose:	Return a visible feature with specified name
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_E_NOT_FOUND - no valid feature found
\*=========================================================================*/
ProError ProUtilFindFeatvisByName(
    ProSolid p_solid,		    /* In : The solid */
    ProName  name,		    /* In : The feature name */ 
    ProFeature *p_feature);	    /* Out: Founded feature,user's memory */


/*=========================================================================*\
    Function:	ProUtilFindFeatureGeomitemByName()
    Purpose:	Return a geomitem with specified name
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_E_NOT_FOUND - no valid feature found
\*=========================================================================*/
ProError ProUtilFindFeatureGeomitemByName( 
    ProFeature	    *p_feature,	    /* In:  The feature handle */
    ProType	    item_type,	    /* In:  The type of expected geometry item */
    ProName  name,		    /* In : The name */ 
    ProGeomitem	    *p_geomitem);   /* Out: The geomitem (user's memory) */



/*=========================================================================*\
    Function:	ProUtilCollectSolidAxisByVector()
    Purpose:	Return a list of axis which belong to the solid and parallel
		(perpendicular) to a vector
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectSolidAxisByVector(
    ProSolid	p_solid,	    /* In:  The handle to the solid */
    ProVector   vector,		    /* In : vector to compare */
    int		option,		    /* In : the option, can be USER_PARALLEL,
					    or USER_PERPENDICULAR */	
    ProAxis	    **p_axis);	    /* Out: ProArray with collected axis . 
					    The function allocates memory 
					    for this argument, but you must 
					    free it. To free the memory, 
					    call the function ProArrayFree()*/

/*=========================================================================*\
    Function:	ProUtilCollectSolidAxisByPlane()
    Purpose:	Return a list of axis which belong to the solid and parallel
		(perpendicular) to a plane surface
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectSolidAxisByPlane(
    ProSolid	p_solid,	    /* In : The handle to the solid */
    ProSurface  plane,		    /* In : The plane */
    int		option,		    /* In : the option, can be USER_PARALLEL,
					    or USER_PERPENDICULAR */	
    ProAxis	    **p_axis);	    /* Out: ProArray with collected axis . 
					    The function allocates memory 
					    for this argument, but you must 
					    free it. To free the memory, 
					    call the function ProArrayFree()*/


/*=========================================================================*\
    Function:	ProUtilCollectProcstep()
    Purpose:    Return a list of process steps in specified solid 	
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectProcstep( 
    ProSolid	    p_solid,	    /* In:  The solid handle */
    ProProcstep	    **p_procstep    /* Out: ProArray with collected proc steps 
					    The function allocates 
					    memory for this argument, but 
					    you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
);

/*=========================================================================*\
    Function:   ProUtilCollectWindowsIds()
    Purpose:    Return a list of active windows
    Returns:    PRO_TK_NO_ERROR - success;
                PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectWindowIds(
    int     **p_winid    /* Out: ProArray with collected win ids
                                            The function allocates
                                            memory for this argument, but
                                            you must free it. To free
                                            the memory, call the function
                                            ProArrayFree() */
);


/*=========================================================================*\
    Function:	ProUtilCollectFamtableInstance()
    Purpose:	Return a list of instance in famtable
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectFamtableInstance( 
    ProFamtable     *p_famtable,     /* In:  The famtable */
    ProFaminstance **famtable_instance /* Out: ProArray with famtable items.
					    The function allocates 
					    memory for this argument, but 
					    you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
);

/*=========================================================================*\
    Function:	ProUtilCollectDtmCurveFeat()
    Purpose:	Return a list of features in the solid
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectDtmCurveFeat( 
    ProSolid	    p_solid,	    /* In:  The solid handle */
    ProFeature	    **p_features    /* Out: ProArray with collected features
					    items. The function allocates 
					    memory for this argument, but 
					    you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
);

/*=========================================================================*\
    Function:	ProUtilCollectFamtableItem()
    Purpose:	Return a list of items in famtable
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectFamtableItem( 
    ProFamtable     *p_famtable,     /* In:  The famtable */
    ProFamtableItem **famtable_item /* Out: ProArray with famtable items.
					    The function allocates 
					    memory for this argument, but 
					    you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
);

/*=========================================================================*\
    Function:	ProUtilCollectExplstate()
    Purpose:	Return a list of exploded in assembly
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectExplstate( 
    ProAssembly     p_assembly,    /* In:  The assembly */
    ProGeomitem     **p_geomitems   /* Out: ProArray with collected geomitems.
					    The function allocates 
					    memory for this argument, but 
					    you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
);

/*=========================================================================*\
    Function:	ProUtilCollectSimprepsdataitems()
    Purpose:	Return a list of simplified representations in the solid
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectSimprepsdataitems( 
    ProSimprepdata   *p_simprepdata,/* In:  The simprepdata handle */
    ProSimprepitem    **p_simprepitems    /* Out: ProArray with collected simp reps
					    items. The function allocates 
					    memory for this argument, but 
					    you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
);

/*=========================================================================*\
    Function:	ProUtilCollectRelset()
    Purpose:	Return a list of relsets
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectRelset( 
    ProMdl	    p_mdl,	    /* In:  The model */
    ProRelset	    **p_relset	    /* Out: ProArray with collected relsets. 
					    The function allocates 
					    memory for this argument, but 
					    you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
);

/*=========================================================================*\
    Function:	ProUtilCollectNCSeqFeat()
    Purpose:	Return a list of NCSeq in the mfg
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectNCSeqFeat( 
    ProSolid	    p_solid,	    /* In:  The solid handle */
    ProFeature	    **p_features    /* Out: ProArray with collected features
					    items. The function allocates 
					    memory for this argument, but 
					    you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
);

/*====================================================================*\
    FUNCTION :	ProUtilCollectAsmcomppath()
    PURPOSE  :	Collect asmcomppathes for all levels of assembly,
		including parts and subassemblies
\*====================================================================*/
ProError ProUtilCollectAsmcomppath(
    ProAssembly assy,		/* In : Assembly */
    ProAsmcomppath **pp_path,	/* Out: The ProArray of collected pathes.
					The function allocates memory for 
					this argument, but you must free it. 
					To free the memory, call the function 
					ProArrayFree() */
    int *p_n_pathes);		/* Out: The number of pathes in the array */

/*====================================================================*\
    FUNCTION :	ProUtilCollectSolidDtmPnts()
    PURPOSE  :	Collect solid's  datum points. The solid can be part or 
		assembly. In case of assebly all level points will be 
		collected
\*====================================================================*/
ProError ProUtilCollectSolidDtmPnts(
    ProSolid solid,	    /* In : solid */
    ProSelection **pp_sel,  /* Out: ProArray of Selection. The function 
				    allocates memory for this argument, but 
				    you must free it. To free the memory, 
				    call the function 
				    ProUtilCollectionArrayFree() */
    int *p_n_pnts);	    /* Out: The number of points in the array */ 

/*====================================================================*\
    FUNCTION :	ProUtilSelectionArrayFree()
    PURPOSE  :	Free ProArray of ProSelection allocated by collection 
\*====================================================================*/
ProError ProUtilSelectionArrayFree(
    ProSelection **pp_sel);  /* In:ProArray  to free memory */

/*=========================================================================*\
    Function:	ProUtilFindCsysByName()
    Purpose:	Return a csys with specified name
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_E_NOT_FOUND - no valid feature found
\*=========================================================================*/
ProError ProUtilFindCsysByName(
    ProSolid p_solid,		    /* In : The solid */
    ProName  name,		    /* In : The feature name */ 
    ProGeomitem *p_geomitem);	    /* Out: Founded csys,user's memory */

/*=========================================================================*\
    Function:	ProUtilFindAxisByName()
    Purpose:	Return a axis with specified name
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_E_NOT_FOUND - no valid feature found
\*=========================================================================*/
ProError ProUtilFindAxisByName(
    ProSolid p_solid,		    /* In : The solid */
    ProName  name,		    /* In : The feature name */ 
    ProGeomitem *p_geomitem);	    /* Out: Founded axis,user's memory */

/*=========================================================================*\
    Function:	ProUtilFindSurfaceByName()
    Purpose:	Return a surface with specified name
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_E_NOT_FOUND - no valid feature found
\*=========================================================================*/
ProError ProUtilFindSurfaceByName(
    ProSolid p_solid,		    /* In : The solid */
    ProName  name,		    /* In : The feature name */ 
    ProGeomitem *p_geomitem);	    /* Out: Founded surface,user's memory */



/*=========================================================================*\
    Function:   ProUtilCollectSolidXsec()
    Purpose:    Return a list of xsecs in specified solid
    Returns:    PRO_TK_NO_ERROR - success;
                PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectSolidXsec(
    ProSolid        p_solid,        /* In:  The solid handle */
    ProXsec         **p_xsec        /* Out: ProArray with collected xsecs
                                            The function allocates
                                            memory for this argument, but
                                            you must free it. To free
                                            the memory, call the function
                                            ProArrayFree() */
);

/*=========================================================================*\
    Function:   ProUtilCollectSolidFeaturesWithFilter()
    Purpose:    Return a list of features in the solid
    Returns:    PRO_TK_NO_ERROR - success;
                PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectSolidFeaturesWithFilter( 
    ProSolid        p_solid,        /* In:  The solid handle */
    ProFeatureFilterAction     filter,  /* In:  Filter function */
    ProFeature      **p_features    /* Out: ProArray with collected features
                                            items. The function allocates 
                                            memory for this argument, but 
                                            you must free it. To free 
                                            the memory, call the function 
                                            ProArrayFree() */
);

/*=========================================================================*\
    Function:   ProUtilCollectSolidAnalysis()
    Purpose:    Return a list of analyses
    Returns:    PRO_TK_NO_ERROR - success;
               PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectSolidAnalysis(
    ProSolid        solid,          /* In : The solid handle*/
    ProAnalysis     **p_analyses    /* Out: ProArray with collected analyses
                                            The function allocates
                                            memory for this argument, but
                                            you must free it. To free
                                            the memory, call the function
                                            ProArrayFree() */
);

#endif    /* PROUTILCOLLECTION_H */
