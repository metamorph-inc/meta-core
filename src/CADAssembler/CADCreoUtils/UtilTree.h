
#ifndef UTILTREE_H
#define UTILTREE_H

#include <ProSelection.h>
#include <ProValue.h>
#include <ProElement.h>
#include <ProItemerr.h>
#include <ProSection.h>
#include <iostream>

typedef enum elem_value_data_type
{
  ELEM_VALUE_TYPE_INT           = 1,
  ELEM_VALUE_TYPE_DOUBLE,
  ELEM_VALUE_TYPE_POINTER,
  ELEM_VALUE_TYPE_STRING,
  ELEM_VALUE_TYPE_WSTRING,
  ELEM_VALUE_TYPE_SELECTION,
  ELEM_VALUE_TYPE_TRANSFORM,
  ELEM_VALUE_TYPE_BOOLEAN,
  ELEM_VALUE_TYPE_COLLECTION
} UtilElemType;

typedef struct element_value_data
{
  UtilElemType   type;
  union
  {
    int            i;   /* integer */
    double         d;   /* double  */
    void          *p;   /* pointer */
    char          *s;   /* string  */
    wchar_t       *w;   /* wide string */
    ProSelection   r;   /* selection */
    double       **t;   /* transform a matrix of 4x4 */
    ProBoolean     b;	/* boolean */
    ProCollection collection; /*collection*/
  } val;
} UtilElemData;


typedef struct
{
   ProPoint2d point;
   double decrease;
} UtilGrWindowData;

typedef enum
{
    PRO_TEST_NO_WINDOW,
    PRO_TEST_GRAPHICS_WINDOW,
    PRO_TEST_INFO_WINDOW
} UtilTreeprintWindow;

typedef struct elem_tree_data
{
    int		 level;
    int		 elem_id;
    ProValueData data;
} ElemTreeData;


typedef ProError ProUtilChangeSelection(ProSelection, ProSelection *, 
    ProAppData);

ProError ProUtilElemtreePrint (
    ProElement elem_tree,
    UtilTreeprintWindow window,
    int *grwin_id );
/*
   Purpose:  Prints an element tree

   Input Arguments:
      elem_tree      - The element tree to print
      window         - print in info or graphical window

   Output Arguments:
      grwin_id       - id of a graphical window( may be NULL)

   Return Values:
      PRO_TK_NO_ERROR    - The tree was sucessfully printed
      PRO_TK_BAD_INPUTS  - One or more of the input arguments are invalid.
*/

ProError ProUtilShowTreeInInfo(
    ProElement elem_tree );

/*====================================================================*\
FUNCTION : ProUtilElemtreeReplaceSelection
PURPOSE  : Changes selection to 'internal' features to original geometry
\*====================================================================*/
ProError ProUtilElemtreeReplaceSelection(
    ProElement elem_tree,
    ProUtilChangeSelection,
    ProAppData);

/*====================================================================*\
FUNCTION : ProUtilOriginalGeomSelection
PURPOSE  : Changes selection to 'internal' surfaces to original geometry
\*====================================================================*/
ProError ProUtilOriginalGeomSelection(
    ProSelection old_sel,
    ProSelection *p_new_sel,
    ProAppData  dummy);

/*====================================================================*\
FUNCTION : ProUtilSelectionChangeAsmcomppath
PURPOSE  : Move selection from part and subassembly refs to the root 
	   assembly level 
\*====================================================================*/
ProError ProUtilSelectionChangeAsmcomppath(
    ProSelection old_sel,
    ProSelection *p_new_sel,
    ProAppData p_asmcomppath);

/*=============================================================*\
  Function: 	ProUtilElemTreeCreate
  Purpose:	Create element tree by the ElemTreeData structure
  Return :      PRO_TK_NO_ERROR if successfull,
\*=============================================================*/
ProError ProUtilElemtreeCreate(
    ElemTreeData *in_elem_arr,	    /* In : The array of ElemTreeData */
    int in_n_elem,	     		    /* In : number of ElemTreeData in elemarr */
    ProElement in_base_tree,	    /* In : base tree, can be NULL */
    ProElement *out_elem_tree);	    /* Out : element tree */


/*=============================================================*\
  Function: 	ProUtilElemtreeElementGet
  Purpose:	Find Element by array of elempathitems
\*=============================================================*/
ProError ProUtilElemtreeElementGet(
    ProElement elem_tree,
    ProElempathItem *elempath_data,
    int	path_size,
    ProElement *p_element);


/*====================================================================*\
FUNCTION : ProUtilFeatErrsPrint
PURPOSE  : Prints feature errors
\*====================================================================*/
ProError ProUtilFeatErrsPrint(ProErrorlist *errs);

std::ostream& ProUtilFeatErrsSerialize( std::ostream& out, ProErrorlist* errs );

/*====================================================================*\
FUNCTION : ProUtilElementValueSet()
PURPOSE  : Replace an element value by new one
\*====================================================================*/
ProError ProUtilElementValueSet(
    ProElement element,
    ProValueData  *value_data);

/*====================================================================*\
FUNCTION : ProUtilElementFeatTypeGet()
PURPOSE  : Find the feature type from the element id
\*====================================================================*/
ProError ProUtilElementFeatTypeGet(ProFeature *feat, ProElemId *name_id);

/*====================================================================*\
FUNCTION : ProUtilElementValueGet()
PURPOSE  : Get the element value from the element type
\*====================================================================*/
ProError ProUtilElementValueGet(ProElement element, UtilElemData *elem_data);

/*====================================================================*\
FUNCTION : ProUtilElementFeatDataGet()
PURPOSE  : Get the element data from the element tree path
\*====================================================================*/
ProError ProUtilElementFeatDataGet(ProFeature feature, 
                            ProElempathItem ref_path_items[],
                            int num_path_ele,
                            UtilElemData *elem_data);

/*====================================================================*\
FUNCTION : ProUtilElemtreeElementAdd()
PURPOSE  : Utility to add the element in an element tree
           It uses all the new updated APIs to set the elment data
\*====================================================================*/
ProError ProUtilElemtreeElementAdd( ProElement parent_element, ProElemId child_id_name,
                                 UtilElemType elem_type,
                                 ProAppData app_data);

/*====================================================================*\
FUNCTION : ProUtilElementTreeDataGet()
PURPOSE  : Utility to add the element value data from the element tree
           It uses all the new updated APIs to get the elment data
\*====================================================================*/
ProError ProUtilElementTreeDataGet(ProElement created_elemtree, 
                          ProElempathItem ref_path_items[],
                          int num_path_ele,
                          UtilElemData *elem_data);                                 

/*====================================================================*\
FUNCTION : ProUtilSketchFeatureReset()
PURPOSE  : Reset a section to a feature
\*====================================================================*/
ProError ProUtilSketchFeatureReset(ProFeature* feat, ProSection *section);

/*====================================================================*\
FUNCTION : ProUtilFeatureSectionGet()
PURPOSE  : Get the section from a feature
\*====================================================================*/
ProError ProUtilFeatureSectionGet(ProFeature feature, ProSection *section);  

/*====================================================================*\
FUNCTION : ProUtilElementValueDataFree()
PURPOSE  : Free the element data
\*====================================================================*/
ProError ProUtilElementValueDataFree(UtilElemData *elem_data);


#endif
