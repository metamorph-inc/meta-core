
#ifndef UTILVISIT_H
#define UTILVISIT_H

#include <ProToolkit.h>
#include <ProAssembly.h>
#include <ProFeature.h>
#include <ProObjects.h>

/*--------------------------------------------------------------------*\
Application macros
\*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*\
Application data types
\*--------------------------------------------------------------------*/
typedef struct tag_find_by_name
{
    ProModelitem *p_modelitem;		 /* The modelitem, user's memory) */
    wchar_t     *name;			 /* The name */
} FindByName;

typedef struct tag_collect_axis_by_vector
{
    ProAxis     **p_axis;		/* The axis collection */ 
    ProVector	vec;			/* The cector to compare */
    int	option;				/* The option for compare */
} CollectAxisByVector;


/*--------------------------------------------------------------------*\
    Function prototypes
\*--------------------------------------------------------------------*/
ProError ProUtilAsmTraverse ( 
	ProAssembly     assembly,
       	ProFunction     action,
	ProAppData      app_data );
/*
    Purpose :		A general utility function which visits all
    			components of an assembly calling the action 
			function given, for each component visited. If
			the action function returns a non-PRO_TK_NO_ERROR
			status, visiting will stop.

    Input Arguments:
	assembly	- the top-level assembly to be traversed
	action		- the action function to be called
	app_data	- the user data to be passed to the action function

    Return Value:
	PRO_TK_NO_ERROR	- all components were visited sucessfully
	other		- the action function returned a non-zero status
*/

ProError ProUtilAsmFeatsTraverse ( 
	 ProAssembly     assembly,
       	 ProFunction     action,
	 ProAppData      app_data );
/*
    Purpose :		A general utility function which visits *all* 
    			features of an assembly recursively calling the 
			action function given, for each component visited. If
			the action function returns a non-PRO_TK_NO_ERROR
			status, visiting will stop.

    Input Arguments:
	assembly	- the top-level assembly to be traversed
	action		- the action function to be called
	app_data	- the user data to be passed to the action function

    Return Value:
	PRO_TK_NO_ERROR	- all features were visited sucessfully
	other		- the action function returned a non-zero status
*/

typedef ProError (*ProUtilAsmCompAct)( ProFeature *component,
				       ProError status,
				       ProAppData app_data);
/*
    Purpose:
	Type for the assembly component visit action passed to
	ProUtilAsmCompVisit()

    Input Arguments:
	component	- The component (also a feature)
	status		- ?????
	app_data	- General data

    Return Value
	PRO_TK_NO_ERROR	- Continue visiting assembly components
	other		- Quit visiting
*/

extern ProError ProUtilFeatvisVisit(ProSolid	solid,
				    ProFunction action,
				    ProAppData  app_data );
/*
    Purpose:	
	Visit the visible features only

    Input Arguments:
	solid		- The solid	
	action		- The function to be called for each feature

    Return Value:
	PRO_TK_NO_ERROR		- visits completed successfully
	PRO_TK_E_NOT_FOUND	- there were no visible features in the solid
	other			- the non zero value returned from the 
			          action function which caused a quit from
				  the visits
*/


extern ProError ProUtilAsmcompVisit( ProAssembly assembly,
				     ProFunction action,
				     ProAppData  app_data);
/*
    Purpose:	
	Visit the components of an assembly

    Input Arguments:
	assembly	- The assembly
	action		- The function to be called for each component

    Return Value:
	PRO_TK_NO_ERROR		- visits completed successfully
	PRO_TK_E_NOT_FOUND	- there were no components in the assembly
	other			- the non zero value returned from the 
			          action function which caused a quit from
				  the visits
*/

extern ProError ProUtilGeomitemactiveVisit( ProFeature *p_feature,
					    ProType item_type,
					    ProFunction action,
					    ProAppData  app_data);
/*
    Purpose:	
	Visit the com

    Input Arguments:
	p_feature	- The feature
	item_type	- filter as in func ProFeatureGeomitemVisit
	action		- The function to be called for each component
	appdata		- not used

    Return Value:
	PRO_TK_NO_ERROR		- visits completed successfully
	PRO_TK_E_NOT_FOUND	- there were no components in the assembly
	other			- the non zero value returned from the 
			          action function which caused a quit from
				  the visits
*/


extern ProError ProUtilAsmFeatsTraverse( 	ProAssembly assembly,
    						ProFunction action,
    						ProAppData app_data);
/*
    Purpose: Visit function for features of an assembly at *ALL* levels
    Input Arguments:
	assembly	-The assembly to visit
	action		-the action to execute on all features
	app_data	-the data used by the action function
    Return Value:
	PRO_TK_NO_ERROR
*/



extern ProError ProUtilAsmcompFilterAction( ProFeature *feature,
					    ProAppData  app_data );
/*
    Purpose:	
	Filter out non-component features

    Input Arguments:
	feature		- Feature to test
	appdata		- Application data (unused - should be null)

    Return Value:
	PRO_TK_NO_ERROR		- Feature is a component
	PRO_TK_E_NOT_FOUND	- Feature is not a component
*/


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
    Function:	ProUtilGeomitemactiveFilterAction()
    Purpose:	Filter function for ProFeatureGeomitemVisit, allow to visit 
		only active geomitems
    Returns:	PRO_TK_NO_ERROR - feature visible
		PRO_TK_CONTINUE - otherwise
\*=========================================================================*/
ProError ProUtilGeomitemactiveFilterAction(
    ProGeomitem *p_geomitem,	/* In : the geomitem */
    ProAppData app_data);	/* In : appdata (not used) */


/*=========================================================================*\
    Function:	ProUtilByNameFilterAction()
    Purpose:	Allow to visit only modelitem with a specified name
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_CONTINUE - no valid modelitem found
\*=========================================================================*/
ProError ProUtilByNameFilterAction(
    ProModelitem *p_modelitem,	    /* In : The modelitem */
    FindByName   *p_findbyname);    /* In : The find structure */


/*=========================================================================*\
    Function:	ProUtilFeatvisByNameFilterAction()
    Purpose:	Allow to visit only visible features with a specified name
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_CONTINUE - no valid feature found found
\*=========================================================================*/
ProError ProUtilFeatvisByNameFilterAction(
    ProFeature   *p_feature,	    /* In : The feature */
    FindByName   *p_findbyname);    /* In : The find structure */


/*=========================================================================*\
    Function:	ProUtilAxisByVectorFilterAction()
    Purpose:	Allow to visit only axis which parallel (perpendicular)
		to a vector
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_CONTINUE - no valid axis found
\*=========================================================================*/
ProError ProUtilAxisByVectorFilterAction(
    ProAxis axis,				/* In : The Axis */
    CollectAxisByVector *p_find_by_vector);	/* In : find data */

/*=========================================================================*\
    Function:	ProUtilCollectDtmCurveFeatFilter()
    Purpose:	Allow to visit only dtm curve features
    Returns:	PRO_TK_NO_ERROR - success;
    		PRO_TK_CONTINUE - feature is not Datum curve
\*=========================================================================*/
ProError ProUtilCollectNCSeqFeatFilter(
    ProFeature   *p_feat,	    /* In : The feature */
    ProAppData   tmp_p_nc_seq);	    /* In : The find structure */
    
/*=========================================================================*\
    Function:	ProUtilCollectDtmCurveFeatFilter()
    Purpose:	Allow to visit only dtm curve features
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_CONTINUE - feature is not NC Seq
\*=========================================================================*/
ProError ProUtilCollectDtmCurveFeatFilter(
    ProFeature   *p_feature,	    /* In : The feature */
    ProAppData   app_data);	    /* In : The find structure */

/*====================================================================*\
    FUNCTION :	ProUtilCollectDtmPointFeatFilter()
    PURPOSE  :	A filter used to visit only Datum Points features
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_CONTINUE - feature is not Datum Point
\*====================================================================*/
ProError ProUtilCollectDtmPointFeatFilter(
    ProFeature *feature,
    ProAppData app_data);

/*=========================================================================*\
    Function:	ProUtilGeomitemByNameFilterAction()
    Purpose:	Allow to visit only geomitem with a specified name
    Returns:	PRO_TK_NO_ERROR - success;
		PRO_TK_CONTINUE - no valid modelitem found
\*=========================================================================*/
ProError ProUtilGeomitemByNameFilterAction(
    ProAppData geomitem,	    /* In : The geomitem */
    FindByName   *p_findbyname);    /* In : The find structure */

/*=============================================================*\
   FUNCTION: ProUtilCheckFeatures
   PURPOSE:  Visit func. Check if another than csys feature is created
\*=============================================================*/
 ProError ProUtilNonCsysFeatCheck(
     ProFeature *feature,
     ProError    status,
     ProAppData  num_csys);   

#endif
