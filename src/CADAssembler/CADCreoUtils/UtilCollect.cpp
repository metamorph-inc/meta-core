


/*----------------------------------------------------------*\
    Headers
\*----------------------------------------------------------*/
#include "ProToolkit.h"
#include "ProArray.h"
#include "ProAxis.h"
#include "ProFeatType.h"
#include "ProGeomitem.h"
#include "ProProcstep.h"
#include "ProExpldstate.h"
#include "ProLayer.h"

#include "TestError.h"
#include "UtilString.h"
#include "UtilMath.h"
#include "UtilCollect.h"
#include "UtilVisit.h"

/*----------------------------------------------------------*\
    Macros
\*----------------------------------------------------------*/
#define ACTION_TYPE( v, f )	\
    p_visit_action_type=v;	\
    p_filter_action_type=f;

/*----------------------------------------------------------*\
    Data types
\*----------------------------------------------------------*/

/*--------------------------------------------------------------------*\
Application data
\*--------------------------------------------------------------------*/
 
/*--------------------------------------------------------------------*\
   These are variables for testing purposes. Since for visiting several
   different objects we can use only one callback function we will check
   this variable for type of the object we visit.
   For example, if we are going to use callbacks of type
   ProAxisFilterAction/ProAxisVisitAction we have to set
     ACTION_TYPE("ProAxisVisitAction", "ProAxisVisitAction")
   before calling visit function.
\*--------------------------------------------------------------------*/
static const char             *p_visit_action_type = "";
static const char             *p_filter_action_type = "";

/*----------------------------------------------------------*\
    Functions declaration (internal)
\*----------------------------------------------------------*/
ProError ProUtilCollect2ParamOpaqueVisitAction(void *, ProAppData);
ProError ProUtilCollect3ParamOpaqueVisitAction(void *, ProError, ProAppData);

ProError ProUtilCollect2ParamDBVisitAction(void *, ProAppData);
ProError ProUtilCollect3ParamDBVisitAction(void *, ProError ,ProAppData);

ProError ProUtilCollect2ParamIdVisitAction(int, ProAppData);

ProError ProUtilDefaultFilter();

ProError ProUtilCollectExtobjReferencesVisitAction( ProExtobj *p_extobj,
    ProWExtobjRef p_extobj_ref, ProError filter_status, 
    ProExtobjrefStatus ref_status, ProAppData app_data );

ProError ProUtilCollectElemtreeElementVisitAction( ProElement p_elemtree, 
    ProElement p_element, ProElempath p_elempath, ProAppData app_data );

ProError ProUtilCollectCurveComponentVisitAction( ProCurve p_curve, 
    ProCurve p_comp, int comp_idx, ProBoolean comp_flip, ProError filter_status, 
    ProAppData app_data );

ProError ProUtilCollectDimVisitAction (ProDimension *dimension,
    ProError status, ProAppData   data);


/*=========================================================================*\
    Function:	ProUtilCollect2ParamDBVisitAction()
    Purpose:	Add any object given by pointer to the Collection
    Returns:	PRO_TK_NO_ERROR - success;
\*=========================================================================*/
ProError ProUtilCollect2ParamDBVisitAction(
    void	    *p_object,	/* In:	The pointer to the object 
					being visited */
    ProAppData	    app_data)	/* In:	In fact it's ProArray** */
{
    ProError status; 
    ProArray *p_array;
    
    p_array = (ProArray*)((void**)app_data)[0];

    TEST_CALL_REPORT( p_visit_action_type, "ProUtilCollect2ParamDBVisitAction",
	PRO_TK_NO_ERROR, 0 );

    status = ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1, p_object );
    return (status);
}

/*=========================================================================*\
    Function:	ProUtilCollect3ParamDBVisitAction()
    Purpose:	Add any object given by pointer to the Collection
    Returns:	PRO_TK_NO_ERROR - success;
\*=========================================================================*/
ProError ProUtilCollect3ParamDBVisitAction(
    void	    *p_object,	/* In:	The pointer to the object 
					being visited */
    ProError	    status,	/* In:  The status returned by filter func */
    ProAppData	    app_data)	/* In:	In fact it's ProArray** */
{
    return (ProUtilCollect2ParamDBVisitAction(p_object, app_data));
}

/*=========================================================================*\
    Function:	ProUtilCollect2ParamOpaqueVisitAction()
    Purpose:	Add any opaque handle to the Collection
    Returns:	PRO_TK_NO_ERROR - success;
\*=========================================================================*/
ProError ProUtilCollect2ParamOpaqueVisitAction(
    void	    *p_object,	/* In:	The opaque handle being visited */
    ProAppData	    app_data)	/* In:	In fact it's ProArray** */
{
    return (ProUtilCollect2ParamDBVisitAction((void*)&p_object, app_data));
}

/*=========================================================================*\
    Function:	ProUtilCollect3ParamOpaqueVisitAction()
    Purpose:	Add any opaque handle to the Collection
    Returns:	PRO_TK_NO_ERROR - success;
\*=========================================================================*/
ProError ProUtilCollect3ParamOpaqueVisitAction(
    void	    *p_object,	/* In:	The opaque handle being visited */
    ProError	    status,	/* In:  The status returned by filter func */
    ProAppData	    app_data)	/* In:	In fact it's ProArray** */
{
    return (ProUtilCollect2ParamDBVisitAction((void*)&p_object, app_data));
}

/*=========================================================================*\
    Function:	ProUtilCollect2ParamIdVisitAction()
    Purpose:	Add any Id to the Collection
    Returns:	PRO_TK_NO_ERROR - success;
\*=========================================================================*/
ProError ProUtilCollect2ParamIdVisitAction(
    int id, 
    ProAppData app_data)
{
    return (ProUtilCollect2ParamDBVisitAction((void*)&id, app_data));
}

/*=========================================================================*\
    Function:	ProUtilDefaultFilter()
    Purpose:	filter function, just write TEST_CALL_REPORT for debug purposes
    Returns:	PRO_TK_NO_ERROR - allow visit all objects;
\*=========================================================================*/
ProError ProUtilDefaultFilter()
{
    TEST_CALL_REPORT( p_filter_action_type, "ProUtilDefaultFilter", 
        PRO_TK_NO_ERROR, 0 );

    return (PRO_TK_NO_ERROR);
}

/*=========================================================================*\
    Function:	ProUtilCollectExtobjReferencesVisitAction()
    Purpose:	Add passed ext objects reference to the collection
    Returns:	PRO_TK_NO_ERROR - success;
\*=========================================================================*/
ProError ProUtilCollectExtobjReferencesVisitAction( 
    ProExtobj		*p_extobj,	/* In:  The handle to the extobj */
    ProWExtobjRef	p_extobj_ref,	/* In:	The reference */
    ProError		filter_status,	/* In:	The status */
    ProExtobjrefStatus	ref_status,	/* In:	Reference status */
    ProAppData		app_data)	/* In:	In fact it's ProArray** */
{
    ExtobjReference    extobj_ref;


    /* Fill in the structure fields */
    extobj_ref.extobj = p_extobj[0];
    extobj_ref.p_extobj_ref = p_extobj_ref;
    extobj_ref.ref_status = ref_status;

    return (ProUtilCollect2ParamDBVisitAction((void*)&extobj_ref, app_data));
}

/*=========================================================================*\
    Function:	ProUtilCollectElemtreeElementVisitAction()
    Purpose:	Add passed element tree element to the collection
    Returns:	PRO_TK_NO_ERROR - success;
\*=========================================================================*/
ProError ProUtilCollectElemtreeElementVisitAction( 
    ProElement	    p_elemtree,	/* In:	The handle to the element tree */
    ProElement	    p_element,	/* In:	The handle to the element */
    ProElempath	    p_elempath,	/* In:	The handle to the elem_path */
    ProAppData	    app_data)	/* In:	In fact it's ElemtreeElement**  */
{
    ElemtreeElement elemtree_element;
    ProError status;
    
    /* Fill in the structure fields */
    elemtree_element.p_element = p_element;
    status = ProElempathAlloc (&(elemtree_element.p_elempath));
    TEST_CALL_REPORT ("ProElempathAlloc()", 
        "ProUtilCollectElemtreeElementVisitAction()", 
        status, status != PRO_TK_NO_ERROR);
    status = ProElempathCopy (p_elempath, elemtree_element.p_elempath);
    TEST_CALL_REPORT ("ProElempathCopy()", 
        "ProUtilCollectElemtreeElementVisitAction()", 
        status, status != PRO_TK_NO_ERROR);
    /* Add the structure to the array */
    return (ProUtilCollect2ParamDBVisitAction((void*)&elemtree_element, 
	app_data));
}

/*=========================================================================*\
    Function:	ProUtilCollectCurveComponentVisitAction()
    Purpose:	Add passed curve component parameters to the collection
    Returns:	PRO_TK_NO_ERROR - success;
\*=========================================================================*/
ProError ProUtilCollectCurveComponentVisitAction( 
    ProCurve     p_curve,   /* In:  The composite curve being processed */
    ProCurve     p_comp,    /* In:  The current component being visited */
    int          comp_idx,  /* In:  The index of the current component in the 
				    curve */
    ProBoolean   comp_flip, /* In:  This is PRO_TK_TRUE if the current component 
				    is flipped */
    ProError     filter_status,	/* In:  The status returned by the filter */
    ProAppData   app_data)  /* In:  In fact it's CurveComponent**  */
{
    CurveComponent  curve_comp;

    /* Fill in the structure fields */
    curve_comp.p_owner = p_curve;
    curve_comp.p_curve = p_comp;
    curve_comp.index = comp_idx;
    curve_comp.flip = comp_flip;

    /* Add the structure to the array */
    return (ProUtilCollect2ParamDBVisitAction((void*)&curve_comp, app_data));
}

/*=========================================================================*\
    Function:	ProUtilFindModelitemVisitAction()
    Purpose:	Copy first visited modelitem to user's memory
    Returns:	PRO_TK_USER_ABORT to stop visiting
\*=========================================================================*/
ProError ProUtilFindModelitemVisitAction(
    ProModelitem *p_modelitem,	    /* In : The modelitem */
    ProError	 status,	    /* In : the status */
    ProModelitem **pp_modelitem)    /* In : The find structure */
{
    TEST_CALL_REPORT( p_visit_action_type, "ProUtilFindModelitemVisitAction()", 
        PRO_TK_NO_ERROR, 0 );
    pp_modelitem[0][0] = p_modelitem[0];
    return (PRO_TK_USER_ABORT);
}


/*=========================================================================*\
    Function:	ProUtilFindGeomitemVisitAction()
    Purpose:	Convert entity to geomitem and call
		ProUtilFindModelitemVisitAction
    Returns:	PRO_TK_USER_ABORT
\*=========================================================================*/
ProError ProUtilFindGeomitemVisitAction(
    ProAppData geomitem,	    /* In : The geomitem */
    ProError	status,		    /* the filter func status */
    FindByName   *p_findbyname)	    /* In : The find structure */
{
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
	status = ProUtilFindModelitemVisitAction(&modelitem, PRO_TK_NO_ERROR,
	    (ProModelitem**)p_findbyname);
    return(status);	    
}

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
)
{
    ProError	    status;

    if( p_geomitems != NULL )
    {
	status=ProArrayAlloc(0,sizeof(ProGeomitem),1,(ProArray*)p_geomitems);
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectExplstate()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProGeomitemAction", "ProGeomitemFilter" )
	    status = ProSolidExpldstateVisit( p_assembly, 
                (ProGeomitemAction)ProUtilCollect3ParamDBVisitAction,
		(ProGeomitemFilter)ProUtilDefaultFilter,
                (ProAppData)&p_geomitems );
	    TEST_CALL_REPORT("ProSolidExpldstateVisit()", 
		"ProUtilCollectExplstate()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_geomitems );
		*p_geomitems = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}

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
)
{
    ProError	    status;

    if( famtable_instance != NULL )
    {
	status=ProArrayAlloc(0,sizeof(ProFaminstance),1,(ProArray*)famtable_instance);
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectFamtableInstance()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProFamtableInstanceAction", 
		"ProFamtableInstanceFilter" )
	    status = ProFamtableInstanceVisit( p_famtable, 
                (ProFamtableInstanceAction)ProUtilCollect3ParamDBVisitAction,
		(ProFamtableInstanceFilter)ProUtilDefaultFilter,
                (ProAppData)&famtable_instance );
	    TEST_CALL_REPORT("ProFamtableInstanceVisit()", 
		"ProUtilCollectFamtableInstance()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)famtable_instance );
		*famtable_instance = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}

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
)
{
    ProError	    status;

    if( famtable_item != NULL )
    {
	status=ProArrayAlloc(0,sizeof(ProFamtableItem),1,(ProArray*)famtable_item);
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectFamtableItem()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProFamtableItemAction", "ProFamtableItemFilter" )
	    status = ProFamtableItemVisit( p_famtable, 
                (ProFamtableItemAction)ProUtilCollect3ParamDBVisitAction,
		(ProFamtableItemFilter)ProUtilDefaultFilter,
                (ProAppData)&famtable_item );
	    TEST_CALL_REPORT("ProFamtableItemVisit()", 
		"ProUtilCollectFamtableItem()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)famtable_item );
		*famtable_item = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}


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
)
{
    ProError	    status;

    if( p_frames != NULL )
    {
	status = ProArrayAlloc(0, sizeof(ProAnimFrame), 1,(ProArray*)p_frames);
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectAnimFrames()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE ("ProAnimFrameAct", "ProAnimFrameAct")
	    status = ProAnimmovieFrameVisit( anim_movie, 
		(ProAnimFrameAct)ProUtilCollect2ParamOpaqueVisitAction,
                (ProAnimFrameAct)ProUtilDefaultFilter,
		(ProAppData)&p_frames );
	    TEST_CALL_REPORT("ProAnimmovieFrameVisit()", 
		"ProUtilCollectAnimFrames()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);

	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_frames );
		*p_frames = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;


    return (status);
}

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
)
{
    ProError	    status;

    if( p_objects != NULL )
    {
	status = ProArrayAlloc(0, sizeof(ProAnimObj), 1, (ProArray*)p_objects);
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectAnimObjects()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE ("ProAnimObjAct", "ProAnimObjAct")
	    status = ProAnimframeObjVisit( anim_frame, 
		(ProAnimObjAct)ProUtilCollect2ParamOpaqueVisitAction,
		(ProAnimObjAct)ProUtilDefaultFilter,
		(ProAppData)&p_objects );
	    TEST_CALL_REPORT("ProAnimframeObjVisit()", 
		"ProUtilCollectAnimObjects()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_objects );
		*p_objects = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}


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
)
{
    ProError	    status;

    if( p_parameters != NULL )
    {
	status=ProArrayAlloc(0,sizeof(ProParameter),1,(ProArray*)p_parameters);
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectParameters()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProParameterAction", "ProParameterFilter" )
	    status = ProParameterVisit( p_modelitem, 
                (ProParameterFilter)ProUtilDefaultFilter,
		(ProParameterAction)ProUtilCollect3ParamDBVisitAction,
                (ProAppData)&p_parameters );
	    TEST_CALL_REPORT("ProParameterVisit()", 
		"ProUtilCollectParameters()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_parameters );
		*p_parameters = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}

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
)
{
#ifndef PT_PRODUCTS_BUILD

    ProError	    status;

    if( p_notes != NULL )
    {
	status = ProArrayAlloc(0, sizeof(ProModelitem), 1, (ProArray*)p_notes);
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectModelNotes()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProMdlNoteVisitAction", "ProMdlNoteFilterAction" )
	    status = ProMdlNoteVisit( p_model, 
		(ProMdlNoteVisitAction)ProUtilCollect3ParamDBVisitAction,
                (ProMdlNoteFilterAction)ProUtilDefaultFilter,
                (ProAppData)&p_notes );
	    TEST_CALL_REPORT("ProMdlNoteVisit()", 
		"ProUtilCollectModelNotes()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_notes );
		*p_notes = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);

#endif
}


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
)
{
    ProError	    status;

    if( p_tools != NULL )
    {
	status = ProArrayAlloc( 0, sizeof(ProTool), 1, (ProArray*)p_tools );
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectMfgTools()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProMfgToolAction", "" )
	    status = ProMfgToolVisit( p_mfg,
		(ProMfgToolAction)ProUtilCollect2ParamDBVisitAction,
                (ProAppData)&p_tools );
	    TEST_CALL_REPORT("ProMfgToolVisit()", 
		"ProUtilCollectMfgTools()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_tools );
		*p_tools = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}

/*=========================================================================*\
    Function:	ProUtilCollectRelset()
    Purpose:	Return a list of tools in the manufacturing model
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
)
{
    ProError	    status;

    if( p_relset != NULL )
    {
	status = ProArrayAlloc( 0, sizeof(ProRelset), 1, (ProArray*)p_relset );
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectRelset()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProSolidRelsetVisitAction", "" )
	    status = ProSolidRelsetVisit( p_mdl,
		(ProSolidRelsetVisitAction)ProUtilCollect2ParamDBVisitAction,
                (ProAppData)&p_relset );
	    TEST_CALL_REPORT("ProSolidRelsetVisit()", 
		"ProUtilCollectRelset()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_relset );
		*p_relset = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}

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
)
{
    ProError	    status;

    if( p_geomitems != NULL )
    {
	status = ProArrayAlloc( 0, sizeof(ProGeomitem), 1, 
	    (ProArray*)p_geomitems );
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectFeatureGeomitems()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProGeomitemAction", "ProGeomitemFilter" )
	    status = ProFeatureGeomitemVisit( p_feature, item_type, 
		(ProGeomitemAction)ProUtilCollect3ParamDBVisitAction,
                (ProGeomitemFilter)ProUtilDefaultFilter,
                (ProAppData)&p_geomitems );
	    TEST_CALL_REPORT("ProFeatureGeomitemVisit()", 
		"ProUtilCollectFeatureGeomitems()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_geomitems );
		*p_geomitems = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}

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
)
{
    ProError	    status;

    if( p_extobj_refs != NULL )
    {
	status = ProArrayAlloc( 0, sizeof(ExtobjReference), 1, 
	    (ProArray*)p_extobj_refs );
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectExtobjReferences()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProExtobjRefVisitAction", 
		"ProExtobjRefFilterAction" )
	    status = ProExtobjRefVisit( p_extobj, 
		(ProExtobjRefVisitAction)ProUtilCollectExtobjReferencesVisitAction,
                (ProExtobjRefFilterAction)ProUtilDefaultFilter,
                (ProAppData)&p_extobj_refs );
	    TEST_CALL_REPORT( "ProExtobjRefVisit()",
		"ProUtilCollectExtobjReferences()", status,
		(status != PRO_TK_NO_ERROR) && 
		(status != PRO_TK_GENERAL_ERROR) &&
		(status != PRO_TK_E_NOT_FOUND) );
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_extobj_refs );
		*p_extobj_refs = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}

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
)
{
    ProError	    status;

    if( p_extobjs != NULL )
    {
	status = ProArrayAlloc( 0, sizeof(ProExtobj), 1, 
	    (ProArray*)p_extobjs );
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectExtobj()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProExtobjVisitAction", "ProExtobjFilterAction" )
	    status = ProExtobjVisit( p_model, p_extobj_class, 
		(ProExtobjVisitAction)ProUtilCollect3ParamDBVisitAction,
                (ProExtobjFilterAction)ProUtilDefaultFilter,
                (ProAppData)&p_extobjs );
	    TEST_CALL_REPORT("ProExtobjVisit()", 
		"ProUtilCollectExtobj()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_extobjs );
		*p_extobjs = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}




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
					    call the function ProArrayFree()*/
)
{
    ProError	    status;

    if( p_elements != NULL )
    {
	status = ProArrayAlloc( 0, sizeof(ElemtreeElement), 1, 
	    (ProArray*)p_elements );
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectElemtreeElements()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProElemtreeVisitAction", "ProElemtreeVisitFilter" )
	    status = ProElemtreeElementVisit( p_elemtree, p_elempath,
	        (ProElemtreeVisitFilter)ProUtilDefaultFilter, 
		(ProElemtreeVisitAction)ProUtilCollectElemtreeElementVisitAction,
                (ProAppData)&p_elements );
	    TEST_CALL_REPORT("ProElemtreeElementVisit()", 
		"ProUtilCollectElemtreeElements()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_elements );
		*p_elements = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}

/*=========================================================================*\
    Function:	ProUtilElemtreeElementArrayFree()
    Purpose:	Free memory allocated by ProUtilCollectElemtreeElements
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilElemtreeElementArrayFree(
    ElemtreeElement **p_elements)
{
   int i, num;
   ProError status;
   
   status = ProArraySizeGet ((ProArray)*p_elements, &num);
   TEST_CALL_REPORT ("ProArraySizeGet()", 
       "ProUtilElemtreeElementArrayFree()", 
       status, status != PRO_TK_NO_ERROR);
   for (i = 0; i < num; i++)
   {
       status = ProElempathFree (&((*p_elements)[i].p_elempath));
       TEST_CALL_REPORT ("ProElempathFree()", 
           "ProUtilElemtreeElementArrayFree()", 
           status, status != PRO_TK_NO_ERROR);
   }
   status = ProArrayFree ((ProArray*)p_elements);
   TEST_CALL_REPORT ("ProArrayFree()", 
       "ProUtilElemtreeElementArrayFree()", 
       status, status != PRO_TK_NO_ERROR);
   return (status);
}

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
)
{
#ifndef PT_PRODUCTS_BUILD

    ProError	    status;

    if( p_components != NULL )
    {
	status = ProArrayAlloc( 0, sizeof(CurveComponent), 1, 
	    (ProArray*)p_components );
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectCurveComponents()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProCurveCompAction", "ProCurveCompFilter" )
	    status = ProCurveCompVisit( p_curve,
		(ProCurveCompAction)ProUtilCollectCurveComponentVisitAction,
                (ProCurveCompFilter)ProUtilDefaultFilter,
                (ProAppData)&p_components );
	    TEST_CALL_REPORT("ProCurveCompVisit()", 
		"ProUtilCollectCurveComponents()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_components );
		*p_components = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);

#endif
}



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
)
{
    ProError	    status;

    if( p_edges != NULL )
    {
	status = ProArrayAlloc( 0, sizeof(ProEdge), 1, (ProArray*)p_edges );
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectContourEdges()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProContourEdgeVisitAction", 
		"ProContourEdgeFilterAction" )
	    status = ProContourEdgeVisit( p_surface, p_contour, 
		(ProContourEdgeVisitAction)ProUtilCollect3ParamOpaqueVisitAction,
                (ProContourEdgeFilterAction)ProUtilDefaultFilter,
                (ProAppData)&p_edges );
	    TEST_CALL_REPORT("ProContourEdgeVisit()", 
		"ProUtilCollectContourEdges()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_edges );
		*p_edges = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}



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
)
{
    ProError	    status;

    if( p_csys != NULL )
    {
	status = ProArrayAlloc( 0, sizeof(ProCsys), 1, (ProArray*)p_csys );
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectSolidCsys()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProCsysVisitAction", "ProCsysFilterAction" )
	    status = ProSolidCsysVisit( p_solid, 
		(ProCsysVisitAction)ProUtilCollect3ParamOpaqueVisitAction,
                (ProCsysFilterAction)ProUtilDefaultFilter,
                (ProAppData)&p_csys );
	    TEST_CALL_REPORT("ProSolidCsysVisit()", 
		"ProUtilCollectSolidCsys()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_csys );
		*p_csys = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}



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
)
{
    ProError	    status;

    if( p_axis != NULL )
    {
	status = ProArrayAlloc( 0, sizeof(ProAxis), 1, (ProArray*)p_axis );
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectSolidAxis()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProAxisVisitAction", "ProAxisFilterAction" )
	    status = ProSolidAxisVisit( p_solid, 
		(ProAxisVisitAction)ProUtilCollect3ParamOpaqueVisitAction,
                (ProAxisFilterAction)ProUtilDefaultFilter,
                (ProAppData)&p_axis );
	    TEST_CALL_REPORT("ProSolidAxisVisit()", 
		"ProUtilCollectSolidAxys()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_axis );
		*p_axis = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}


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
)
{
#ifndef PT_PRODUCTS_BUILD

    ProError	    status;

    if( p_quilts != NULL )
    {
	status = ProArrayAlloc( 0, sizeof(ProQuilt), 1, (ProArray*)p_quilts );
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectSolidQuilts()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProQuiltVisitAction", "ProQuiltFilterAction" )
	    status = ProSolidQuiltVisit( p_solid, 
		(ProQuiltVisitAction)ProUtilCollect3ParamOpaqueVisitAction,
                (ProQuiltFilterAction)ProUtilDefaultFilter,
                (ProAppData)&p_quilts );
	    TEST_CALL_REPORT("ProSolidQuiltVisit()", 
		"ProUtilCollectSolidQuilts()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_quilts );
		*p_quilts = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
#endif
}


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
)
{
    ProError	    status;

    if( p_surfaces != NULL )
    {
	status = ProArrayAlloc(0,sizeof(ProSurface), 1, (ProArray*)p_surfaces);
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectSolidSurfaces()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProSurfaceVisitAction", "ProSurfaceFilterAction" )
	    status = ProSolidSurfaceVisit( p_solid, 
		(ProSurfaceVisitAction)ProUtilCollect3ParamOpaqueVisitAction,
                (ProSurfaceFilterAction)ProUtilDefaultFilter,
                (ProAppData)&p_surfaces );
	    TEST_CALL_REPORT("ProSolidSurfaceVisit()", 
		"ProUtilCollectSolidSurfaces()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_surfaces );
		*p_surfaces = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}



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
)
{
#ifndef PT_PRODUCTS_BUILD

    ProError	    status;

    if( p_surfaces != NULL )
    {
	status = ProArrayAlloc(0,sizeof(ProSurface), 1, (ProArray*)p_surfaces );
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectQuiltSurfaces()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProQuiltSurfaceVisitAction",
		"ProQuiltSurfaceFilterAction" )
	    status = ProQuiltSurfaceVisit( p_quilt, 
		(ProQuiltSurfaceVisitAction)ProUtilCollect3ParamOpaqueVisitAction,
                (ProQuiltSurfaceFilterAction)ProUtilDefaultFilter,
                (ProAppData)&p_surfaces );
	    TEST_CALL_REPORT("ProQuiltSurfaceVisit()", 
		"ProUtilCollectQuiltSurfaces()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_surfaces );
		*p_surfaces = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);

#endif
}



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
)
{
#ifndef PT_PRODUCTS_BUILD

    ProError	    status;

    if( p_simpreps != NULL )
    {
	status = ProArrayAlloc(0,sizeof(ProSimprep), 1, (ProArray*)p_simpreps);
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectSolidSimpreps()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProGeomitemAction", "ProGeomitemFilter" )
	    status = ProSolidSimprepVisit( p_solid, 
                (ProGeomitemFilter)ProUtilDefaultFilter,
		(ProGeomitemAction)ProUtilCollect3ParamDBVisitAction,
                (ProAppData)&p_simpreps );
	    TEST_CALL_REPORT("ProSolidSimprepVisit()", 
		"ProUtilCollectSolidSimpreps()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_simpreps );
		*p_simpreps = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);

#endif
}

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
)
{
#ifndef PT_PRODUCTS_BUILD

    ProError	    status;

    if( p_simprepitems != NULL )
    {
	status = ProArrayAlloc(0,sizeof(ProSimprepitem), 1, (ProArray*)p_simprepitems);
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectSolidSimpreps()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProFunction", "ProFunction" )
	    status = ProSimprepdataitemsVisit( p_simprepdata, 
                (ProFunction)ProUtilDefaultFilter,
		(ProFunction)ProUtilCollect3ParamDBVisitAction,
                (ProAppData)&p_simprepitems );
	    TEST_CALL_REPORT("ProSolidSimprepVisit()", 
		"ProUtilCollectSolidSimpreps()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_simprepitems );
		*p_simprepitems = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);

#endif
}

/*=========================================================================*\
    Function:	ProUtilCollectSolidFeaturesWithFilter()
    Purpose:	Return a list of features in the solid
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectSolidFeaturesWithFilter( 
    ProSolid	    p_solid,	    /* In:  The solid handle */
    ProFeatureFilterAction     filter,  /* In:  Filter function */
    ProFeature	    **p_features    /* Out: ProArray with collected features
					    items. The function allocates 
					    memory for this argument, but 
					    you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
)
{
    ProError	    status;

    if( p_features != NULL )
    {
	status = ProArrayAlloc(0,sizeof(ProFeature), 1, (ProArray*)p_features);
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectSolidFeatures()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProFeatureVisitAction", "ProFeatureFilterAction" )
	    status = ProSolidFeatVisit( p_solid, 
		(ProFeatureVisitAction)ProUtilCollect3ParamDBVisitAction,
                (ProFeatureFilterAction)filter,
                (ProAppData)&p_features );
	    TEST_CALL_REPORT("ProSolidFeatVisit()", 
		"ProUtilCollectSolidFeatures()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_features );
		*p_features = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}

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
)
{
    ProError	    status;

    status =  ProUtilCollectSolidFeaturesWithFilter (p_solid, 
        (ProFeatureFilterAction)ProUtilDefaultFilter, p_features);
    return (status);
}

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
)
{
    ProError	    status;

    status =  ProUtilCollectSolidFeaturesWithFilter (p_solid, 
        (ProFeatureFilterAction)ProUtilCollectNCSeqFeatFilter, p_features);
    return (status);
}


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
)
{
    ProError	    status;

    status =  ProUtilCollectSolidFeaturesWithFilter (p_solid, 
        (ProFeatureFilterAction)ProUtilCollectDtmCurveFeatFilter, p_features);
    return (status);
}

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
)
{
    ProError	    status;
    
    status =  ProUtilCollectSolidFeaturesWithFilter (p_solid, 
        (ProFeatureFilterAction)ProUtilFeatvisFilterAction, p_features);
    return (status);
}

/*=========================================================================*\
    Function:	ProUtilCollectAsmcomp()
    Purpose:	Return a list of assembly components in the assembly
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
)
{
    ProError	    status;
    
    status =  ProUtilCollectSolidFeaturesWithFilter (p_assy, 
        (ProFeatureFilterAction)ProUtilAsmcompFilterAction, p_asmcomps);

    return (status);
}


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
)
{
    ProError	    status;

    if( p_contours != NULL )
    {
	status = ProArrayAlloc( 0, sizeof(ProContour), 1, 
	    (ProArray*)p_contours );
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectSurfaceContours()",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    ACTION_TYPE( "ProSurfaceContourVisitAction", 
		"ProSurfaceContourFilterAction" )
	    status = ProSurfaceContourVisit( p_surfaces, 
		(ProSurfaceContourVisitAction)ProUtilCollect3ParamOpaqueVisitAction,
                (ProSurfaceContourFilterAction)ProUtilDefaultFilter,
                (ProAppData)&p_contours );
	    TEST_CALL_REPORT("ProSurfaceContourVisit()", 
		"ProUtilCollectSurfaceContours()", status, 
		status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_contours );
		*p_contours = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}


/*=========================================================================*\
    Function:	ProUtilCollectMdlLayers()
    Purpose:	Return a list of layers in the solid
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectMdlLayers( 
    ProMdl	    model,	    /* In:  The model handle */
    ProLayer	    **p_layers      /* Out: ProArray with collected layers.
					    The function allocates 
					    memory for this argument, but 
					    you must free it. To free 
					    the memory, call the function 
					    ProArrayFree() */
)
{
    ProError	    status;

    status = ProMdlLayersCollect (model, p_layers);

    return (status);
}

/*=========================================================================*\
    Function:	ProUtilCollectDimVisitAction()
    Purpose:	Add any dimension given by pointer to the array of dimensions
    Returns:	PRO_TK_NO_ERROR - success;
\*=========================================================================*/
ProError ProUtilCollectDimVisitAction (ProDimension *dimension,
                                       ProError     filter_status, /* ignored */
                                       ProAppData   data)
{
  ProDimension **p_dims = (ProDimension**)data;
  ProDimension cur_dim;

  cur_dim.owner = dimension->owner;
  cur_dim.id = dimension->id;
  cur_dim.type = dimension->type;
  ProArrayObjectAdd((ProArray*)p_dims, PRO_VALUE_UNUSED, 1, &cur_dim);
  
  return PRO_TK_NO_ERROR;
}
/*=========================================================================*\
    Function:	ProUtilCollectDimensions()
    Purpose:	Return a list of dimensions in the part, assembly, or drawing
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectDimension( 
    ProMdl	model,	    /* In:  The handle to the model */
    ProBoolean   refdim,    /* In:  PRO_B_TRUE for collecting reference dimensions,
                                    PRO_B_FALSE for collecting standard dimensions */ 
    ProDimension **pp_dims   /* Out: ProArray with collected dimensions. 
				    The function allocates memory 
				    for this argument, but you must 
				    free it. To free the memory, 
				    call the function ProArrayFree()*/
)
{
    int n;
    ProError status;

    status = ProArrayAlloc( 0, sizeof(ProDimension), 1, (ProArray*)pp_dims);
    TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectDimensions",
                                       status, status != PRO_TK_NO_ERROR);

    if( status == PRO_TK_NO_ERROR )
    {
      status = ProSolidDimensionVisit( (ProSolid)model, refdim,
                (ProDimensionVisitAction)ProUtilCollectDimVisitAction,
                (ProDimensionFilterAction)NULL,
                (ProAppData)pp_dims);
    }




    ProArraySizeGet((ProArray*)pp_dims, &n);
    if (n==0)
    {
      status = PRO_TK_E_NOT_FOUND;
    }

    return (status);

}

/*=========================================================================*\
    Function:	ProUtilFindFeatureByName()
    Purpose:	Return a feature with specified name
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_E_NOT_FOUND - no valid feature found
\*=========================================================================*/
ProError ProUtilFindFeatureByName(
    ProSolid p_solid,		    /* In : The solid */
    ProName  name,		    /* In : The feature name */ 
    ProFeature *p_feature)	    /* Out: Founded feature,user's memory */
{
    FindByName find;
    ProError status;

    find.p_modelitem = (ProModelitem*)p_feature;
    find.name = name;

    ACTION_TYPE( "ProFeatureVisitAction", "ProFeatureFilterAction" )
    status = ProSolidFeatVisit(	(ProSolid)p_solid, 
	(ProFeatureVisitAction)ProUtilFindModelitemVisitAction,
        (ProFeatureFilterAction)ProUtilByNameFilterAction,
        (ProAppData)&find);
    TEST_CALL_REPORT("ProSolidFeatVisit()", 
		    "ProUtilFindFeatureByName()", status, 
		    status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);

/*----------------------------------------------------------------*\
    if feature founded set status to PRO_TK_NO_ERROR
\*----------------------------------------------------------------*/    
    if (status == PRO_TK_USER_ABORT)
	status = PRO_TK_NO_ERROR;
    return (status);
}

/*=========================================================================*\
    Function:	ProUtilFindFeatvisByName()
    Purpose:	Return a visible feature with specified name
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_E_NOT_FOUND - no valid feature found
\*=========================================================================*/
ProError ProUtilFindFeatvisByName(
    ProSolid p_solid,		    /* In : The solid */
    ProName  name,		    /* In : The feature name */ 
    ProFeature *p_feature)	    /* Out: Founded feature,user's memory */
{
    FindByName find;
    ProError status;

    find.p_modelitem = (ProModelitem*)p_feature;
    find.name = name;

    ACTION_TYPE( "ProFeatureVisitAction", "ProFeatureFilterAction" )
    status = ProSolidFeatVisit(	(ProSolid)p_solid, 
	(ProFeatureVisitAction)ProUtilFindModelitemVisitAction,
        (ProFeatureFilterAction)ProUtilFeatvisByNameFilterAction,
        (ProAppData)&find);
    TEST_CALL_REPORT("ProSolidFeatVisit()", 
		    "ProUtilFindFeatvisByName()", status, 
		    status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
/*----------------------------------------------------------------*\
    if feature founded set status to PRO_TK_NO_ERROR
\*----------------------------------------------------------------*/    
    if (status == PRO_TK_USER_ABORT)
	status = PRO_TK_NO_ERROR;
    return (status);
}


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
    ProGeomitem	    *p_geomitem)     /* Out: The geomitem (user's memory) */
{
    FindByName find;
    ProError status;

    find.p_modelitem = (ProModelitem*)p_geomitem;
    find.name = name;

    ACTION_TYPE( "ProGeomitemAction", "ProGeomitemFilter" )
    status = ProFeatureGeomitemVisit( p_feature, item_type, 
	(ProGeomitemAction)ProUtilFindModelitemVisitAction,
        (ProGeomitemFilter)ProUtilByNameFilterAction,
        (ProAppData)&find);

/*----------------------------------------------------------------*\
    if geomitem is founded set status to PRO_TK_NO_ERROR
\*----------------------------------------------------------------*/    
    if (status == PRO_TK_USER_ABORT)
	status = PRO_TK_NO_ERROR;
    return (status);
}

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
    ProAxis	    **p_axis)	    /* Out: ProArray with collected axis . 
					    The function allocates memory 
					    for this argument, but you must 
					    free it. To free the memory, 
					    call the function ProArrayFree()*/
{
    ProError status;
    CollectAxisByVector    find;

    if( p_axis != NULL )
    {
	status = ProArrayAlloc( 0, sizeof(ProAxis), 1, (ProArray*)p_axis);
	TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectSolidAxisByVector",
					    status, status != PRO_TK_NO_ERROR);
	if( status == PRO_TK_NO_ERROR )
	{
	    find.p_axis = p_axis;
	    ProUtilVectorCopy(vector, find.vec);
	    find.option = option;
	    ACTION_TYPE( "ProAxisVisitAction", "ProAxisFilterAction" )
	    status = ProSolidAxisVisit( p_solid, 
		(ProAxisVisitAction)ProUtilCollect3ParamOpaqueVisitAction,
                (ProAxisFilterAction)ProUtilAxisByVectorFilterAction,
                (ProAppData)&find );
            TEST_CALL_REPORT("ProSolidAxisVisit()", 
                "ProUtilCollectSolidAxisByVector",
                status, status != PRO_TK_NO_ERROR);
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_axis );
		*p_axis = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}

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
    ProAxis	    **p_axis)	    /* Out: ProArray with collected axis . 
					    The function allocates memory 
					    for this argument, but you must 
					    free it. To free the memory, 
					    call the function ProArrayFree()*/
{
    ProError   status;
    ProSrftype srf_type;
    ProGeomitemdata *surfdata;
    ProVector  vector;

    status = ProSurfaceTypeGet(plane, &srf_type);
    if (status == PRO_TK_NO_ERROR && srf_type == PRO_SRF_PLANE)
    {
	status = ProSurfaceDataGet(plane, &surfdata);
	ProUtilVectorCopy(surfdata->data.p_surface_data->srf_shape.plane.e3,
	    vector);
	status = ProGeomitemdataFree(&surfdata);
	status = ProUtilCollectSolidAxisByVector(p_solid, vector, option,
		p_axis);
    }
	else status = PRO_TK_BAD_INPUTS;
    return (status);
}

/*=========================================================================*\
    Function:   ProUtilCollectProcstep()
    Purpose:    Return a list of process steps in specified solid
    Returns:    PRO_TK_NO_ERROR - success;
                PRO_TK_BAD_INPUTS - invalid parameters
\*=========================================================================*/
ProError ProUtilCollectProcstep(
    ProSolid        p_solid,        /* In:  The solid handle */
    ProProcstep     **p_procstep    /* Out: ProArray with collected proc steps
                                            The function allocates
                                            memory for this argument, but
                                            you must free it. To free
                                            the memory, call the function
                                            ProArrayFree() */
)
{
    ProError        status;

    if( p_procstep != NULL )
    {
        status = ProArrayAlloc(0,sizeof(ProProcstep), 1, (ProArray*)p_procstep);
        TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectProcstep()",
                                            status, status != PRO_TK_NO_ERROR);
        if( status == PRO_TK_NO_ERROR )
        {
	    ACTION_TYPE( "ProGeomitemAction", "ProGeomitemFilter" )
            status = ProProcstepVisit( p_solid,
                (ProGeomitemAction)ProUtilCollect3ParamDBVisitAction,
                (ProGeomitemFilter)ProUtilDefaultFilter,
                (ProAppData)&p_procstep );
            TEST_CALL_REPORT("ProProcstepVisit()",
                "ProUtilCollectProcstep()", status,
                status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
            if( status != PRO_TK_NO_ERROR )
            {
                ProArrayFree( (ProArray*)p_procstep );
                *p_procstep = NULL;
            }
        }
    }
    else
        status = PRO_TK_BAD_INPUTS;

    return (status);
}


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
)
{
    ProError        status;


    if( p_winid != NULL )
    {
        status = ProArrayAlloc(0, sizeof(int), 1, (ProArray*)p_winid);
        TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectWindowIds()",
                                            status, status != PRO_TK_NO_ERROR);
        if( status == PRO_TK_NO_ERROR )
        {
	    ACTION_TYPE( "ProWindowVisitAction", "" )
            status = ProWindowsVisit(
                (ProWindowVisitAction)ProUtilCollect2ParamIdVisitAction,
                (ProAppData)&p_winid );
            TEST_CALL_REPORT("ProWindowsVisit()",
                "ProUtilCollectWindowIds()", status,
                status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
            if( status != PRO_TK_NO_ERROR )
            {
                ProArrayFree( (ProArray*)p_winid );
                *p_winid= NULL;
            }
        }
    }
    else
        status = PRO_TK_BAD_INPUTS;

    return (status);
}

/*=========================================================================*\
    Function:	ProUtilFindCsysByName()
    Purpose:	Return a csys with specified name
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_E_NOT_FOUND - no valid feature found
\*=========================================================================*/
ProError ProUtilFindCsysByName(
    ProSolid p_solid,		    /* In : The solid */
    ProName  name,		    /* In : The feature name */ 
    ProGeomitem *p_geomitem)	    /* Out: found csys,user's memory */
{
    FindByName find;
    ProError status;
    ProModelitem modelitem;

    modelitem.type = PRO_CSYS;
    modelitem.owner = (ProMdl)p_solid;

    find.p_modelitem = &modelitem;
    find.name = name;

    ACTION_TYPE( "ProCsysVisitAction", "ProCsysFilterAction" )
    status = ProSolidCsysVisit( p_solid, 
	(ProCsysVisitAction)ProUtilFindGeomitemVisitAction,
        (ProCsysFilterAction)ProUtilGeomitemByNameFilterAction,
        (ProAppData)&find );
    TEST_CALL_REPORT("ProSolidCsysVisit()", 
	"ProUtilFindCsysByName()", status, 
	status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);

/*----------------------------------------------------------------*\
    if geomitem found set status to PRO_TK_NO_ERROR
\*----------------------------------------------------------------*/    
    if (status == PRO_TK_USER_ABORT)
    {
	status = PRO_TK_NO_ERROR;
	p_geomitem[0] = *((ProGeomitem*)&modelitem);
    }
    return (status);
}


/*=========================================================================*\
    Function:	ProUtilFindAxisByName()
    Purpose:	Return a axis with specified name
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_E_NOT_FOUND - no valid feature found
\*=========================================================================*/
ProError ProUtilFindAxisByName(
    ProSolid p_solid,		    /* In : The solid */
    ProName  name,		    /* In : The axis name */ 
    ProGeomitem *p_geomitem)	    /* Out: found axis,user's memory */
{
    FindByName find;
    ProError status;
    ProModelitem modelitem;

    modelitem.type = PRO_AXIS;
    modelitem.owner = (ProMdl)p_solid;

    find.p_modelitem = &modelitem;
    find.name = name;

    ACTION_TYPE( "ProAxisVisitAction", "ProAxisFilterAction" )
    status = ProSolidAxisVisit( p_solid, 
	(ProAxisVisitAction)ProUtilFindGeomitemVisitAction,
        (ProAxisFilterAction)ProUtilGeomitemByNameFilterAction,
        (ProAppData)&find );
    TEST_CALL_REPORT("ProSolidAxisVisit()", 
	"ProUtilFindCsysByName()", status, 
	status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);

/*----------------------------------------------------------------*\
    if geomitem found set status to PRO_TK_NO_ERROR
\*----------------------------------------------------------------*/    
    if (status == PRO_TK_USER_ABORT)
    {
	status = PRO_TK_NO_ERROR;
	p_geomitem[0] = *((ProGeomitem*)&modelitem);
    }
    return (status);
}

/*=========================================================================*\
    Function:	ProUtilFindSurfaceByName()
    Purpose:	Return a surface with specified name
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_E_NOT_FOUND - no valid feature found
\*=========================================================================*/
ProError ProUtilFindSurfaceByName(
    ProSolid p_solid,		    /* In : The solid */
    ProName  name,		    /* In : The surface name */ 
    ProGeomitem *p_geomitem)	    /* Out: found surface,user's memory */
{
    FindByName find;
    ProError status;
    ProModelitem modelitem;

    modelitem.type = PRO_SURFACE;
    modelitem.owner = (ProMdl)p_solid;

    find.p_modelitem = &modelitem;
    find.name = name;

    ACTION_TYPE( "ProSurfaceVisitAction", "ProSurfaceFilterAction" )
    status = ProSolidSurfaceVisit( p_solid, 
	(ProSurfaceVisitAction)ProUtilFindGeomitemVisitAction,
        (ProSurfaceFilterAction)ProUtilGeomitemByNameFilterAction,
        (ProAppData)&find );
    TEST_CALL_REPORT("ProSolidSurfaceVisit()", 
	"ProUtilFindCsysByName()", status, 
	status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);

/*----------------------------------------------------------------*\
    if geomitem found set status to PRO_TK_NO_ERROR
\*----------------------------------------------------------------*/    
    if (status == PRO_TK_USER_ABORT)
    {
	status = PRO_TK_NO_ERROR;
	p_geomitem[0] = *((ProGeomitem*)&modelitem);
    }
    return (status);
}



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
)
{
    ProError        status;
    int             n_sec;

    if( p_xsec != NULL )
    {
        status = ProArrayAlloc(0, sizeof(ProXsec), 1, (ProArray*)p_xsec);
        TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectSolidXsec()",
                                            status, status != PRO_TK_NO_ERROR);
        if( status == PRO_TK_NO_ERROR )
        {
	    ACTION_TYPE( "ProSolidXsecVisitAction", "" )
            status = ProSolidXsecVisit( p_solid,
                (ProSolidXsecVisitAction)ProUtilCollect2ParamDBVisitAction,
                (ProAppData)&p_xsec );
            TEST_CALL_REPORT("ProSolidXsecVisit()",
                "ProUtilCollectSolidXsec()", status,
                status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
            /* Here was a bug  
                - ProSolidXsecVisit does not return good status
                Use array size as workaround 
            */
            status = ProArraySizeGet((ProArray)*p_xsec, &n_sec);
            if (n_sec <=0 || status != PRO_TK_NO_ERROR)
            if( status != PRO_TK_NO_ERROR )
            {
                ProArrayFree( (ProArray*)p_xsec );
                *p_xsec = NULL;
            }
        }
    }
    else
        status = PRO_TK_BAD_INPUTS;

    return (status);
}

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
)
{
    ProError        status;

    if( p_analyses != NULL )
    {
        status = ProArrayAlloc(0, sizeof(int), 1, (ProArray*)p_analyses);
        TEST_CALL_REPORT("ProArrayAlloc()", "ProUtilCollectSolidAnalysis()",
            status, status != PRO_TK_NO_ERROR);
        if( status == PRO_TK_NO_ERROR )
        {
	    ACTION_TYPE( "ProAnalysisVisitAction", 
                         "ProAnalysisFilterAction" )
            status = ProSolidAnalysisVisit(
                solid,
                (ProAnalysisVisitAction)ProUtilCollect3ParamOpaqueVisitAction,
                (ProAnalysisFilterAction)ProUtilDefaultFilter,
                (ProAppData)&p_analyses );
            TEST_CALL_REPORT("ProSolidAnalysisVisit()",
                "ProUtilCollectSolidAnalysis()", status,
                status != PRO_TK_NO_ERROR && status != PRO_TK_E_NOT_FOUND);
            if( status != PRO_TK_NO_ERROR )
            {
                ProArrayFree( (ProArray*)p_analyses );
                *p_analyses = NULL;
            }
        }
    }
    else
        status = PRO_TK_BAD_INPUTS;

    return (status);
}
