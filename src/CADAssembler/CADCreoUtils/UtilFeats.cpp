

/*--------------------------------------------------------------------*\
Pro/TOOLKIT includes
\*--------------------------------------------------------------------*/
#include <ProToolkit.h>
#include <ProFeature.h>
#include <ProModelitem.h>
#include <ProArray.h>
#include <ProNcseq.h>
#include <ProUtil.h>
#include <ProFeatType.h>
#include <ProElement.h>
#include <ProTKRunTime.h>
/*--------------------------------------------------------------------*\
Application includes
\*--------------------------------------------------------------------*/
#include "TestError.h"
#include "UtilFeats.h"
#include "UtilCollect.h"
#include "UtilNames.h"
#include "UtilTypes.h"

/*====================================================================*\
    FUNCTION :	ProUtilFeatureDump()
    PURPOSE  :	To write a decription of a feature to a text file
\*====================================================================*/
void ProUtilFeatureDump(
    ProModelitem *feature,
    FILE *fp)
{
    ProError status;
    ProSolid owner;
    ProAsmcomppath path;
    ProModelitem modelitem;    
    int n_children, c, n_parents, p;
    ProPatternStatus pstatus;
    ProGrppatternStatus gpstatus;
    ProFeatStatus fstatus;
    ProGroupStatus gstatus;
    ProSelection selection;
    char model_name[PRO_NAME_SIZE], model_type[PRO_NAME_SIZE];
    int m;
    ProFeattype ftype;
    char ftype_str[PRO_NAME_SIZE];
    int *children_ids, *parents_ids;
    ProBoolean is_nc_seq;
    ProWVerstamp stamp;
    char* p_stamp_string;

/*--------------------------------------------------------------------*\
    Check the feature owner
\*--------------------------------------------------------------------*/
    status = ProModelitemMdlGet(feature, (ProMdl *) &owner);
    TEST_CALL_REPORT("ProModelitemMdlGet()", "ProUtilFeatureDump()",
					status, status != PRO_TK_NO_ERROR);
    ProUtilModelnameGet((ProMdl*)&owner, model_name,  model_type);
    ProTKFprintf(fp,"Owner     : %s.%s\n", model_name, model_type);

/*--------------------------------------------------------------------*\
    Check the feature owner
\*--------------------------------------------------------------------*/
    status = ProFeatureSolidGet(feature, &owner);
    TEST_CALL_REPORT("ProFeatureSolidGet()", "ProUtilFeatureDump()",
					status, status != PRO_TK_NO_ERROR);
    ProUtilModelnameGet((ProMdl*)&owner, model_name,  model_type);
    ProTKFprintf(fp,"Sld Owner : %s.%s\n", model_name, model_type);

/*--------------------------------------------------------------------*\
    Get the list of children of the selected feature
\*--------------------------------------------------------------------*/
    status = ProFeatureChildrenGet(feature, &children_ids, &n_children);
    TEST_CALL_REPORT("ProFeatureChildrenGet()", "ProUtilFeatureDump()",
					status, status != PRO_TK_NO_ERROR);
    ProTKFprintf(fp, "Children : ");
    for(c=0; c<n_children; c++)
	ProTKFprintf(fp,"%d ", children_ids[c]);
    ProTKFprintf(fp,"\n");

/*--------------------------------------------------------------------*\
    Get the list of parents of the selected feature
\*--------------------------------------------------------------------*/
    status = ProFeatureParentsGet(feature, &parents_ids, &n_parents);
    TEST_CALL_REPORT("ProFeatureParentsGet()", "ProUtilFeatureDump()",
					status, status != PRO_TK_NO_ERROR);
    ProTKFprintf(fp, "Parents  : ");
    for(p=0; p<n_parents; p++)
	ProTKFprintf(fp,"%d ", parents_ids[p]);
    ProTKFprintf(fp,"\n");

/*--------------------------------------------------------------------*\
    Get the version stamp of the specified feature
\*--------------------------------------------------------------------*/
    status = ProVerstampAlloc( &stamp );
    TEST_CALL_REPORT("ProVerstampAlloc()", "ProUtilFeatureDump()",
					status, status != PRO_TK_NO_ERROR);
    status = ProFeatureVerstampGet( feature, &stamp );
    TEST_CALL_REPORT("ProFeatureVerstampGet()", "ProUtilFeatureDump()",
					status, status != PRO_TK_NO_ERROR);
    status = ProVerstampStringGet( stamp, &p_stamp_string );
    TEST_CALL_REPORT("ProVerstampStringGet()", "ProUtilFeatureDump()",
					status, status != PRO_TK_NO_ERROR);
    
    ProTKFprintf( fp,"Version stamp: %s\n", p_stamp_string );

    status = ProVerstampStringFree( &p_stamp_string );
    TEST_CALL_REPORT("ProVerstampStringFree()", "ProUtilFeatureDump()",
					status, status != PRO_TK_NO_ERROR);
    status = ProVerstampFree( &stamp );
    TEST_CALL_REPORT("ProVerstampFree()", "ProUtilFeatureDump()",
					status, status != PRO_TK_NO_ERROR);

/*--------------------------------------------------------------------*\
    Get the feature status
\*--------------------------------------------------------------------*/
    status = ProFeatureStatusGet(feature, &fstatus);
    TEST_CALL_REPORT("ProFeatureStatusGet()", "ProUtilFeatureDump()",
					status, status != PRO_TK_NO_ERROR);
    ProTKFprintf(fp, "FEATURE STATUS       ");
    switch(fstatus)
    {
    case PRO_FEAT_INVALID:
	ProTKFprintf(fp, "Invalid\n");
	break;
    case PRO_FEAT_ACTIVE:
	ProTKFprintf(fp, "Active\n");
	break;
    case PRO_FEAT_INACTIVE:
	ProTKFprintf(fp, "Inactive\n");
	break;
    case PRO_FEAT_FAMTAB_SUPPRESSED:
	ProTKFprintf(fp, "Famtab suppressed\n");
	break;
    case PRO_FEAT_SIMP_REP_SUPPRESSED:
	ProTKFprintf(fp,"Simprep suppressed\n");
	break;
    case PRO_FEAT_PROG_SUPPRESSED:
	ProTKFprintf(fp,"Pro/PROGRAM suppressed\n");
	break;
    case PRO_FEAT_SUPPRESSED:
	ProTKFprintf(fp, "Suppressed\n");
	break;
    case PRO_FEAT_UNREGENERATED:
	ProTKFprintf(fp, "Unregenerated\n");
	break;
    default:
	ProTKFprintf(fp, "**UNKNOWN**\n");
	break;
    }

/*--------------------------------------------------------------------*\
    Get the group status of the feature
\*--------------------------------------------------------------------*/
    status = ProFeatureGroupStatusGet( feature, &gstatus );
    TEST_CALL_REPORT("ProFeatureGroupStatusGet()", "ProUtilFeatureDump()",
                                        status, status != PRO_TK_NO_ERROR);
    ProTKFprintf(fp, "GROUP STATUS       ");
    switch(gstatus)
    {
    case PRO_GROUP_INVALID:
	ProTKFprintf(fp, "Invalid\n");
	break;
    case PRO_GROUP_NONE:
	ProTKFprintf(fp, "None\n");
	break;
    case PRO_GROUP_MEMBER:
	ProTKFprintf(fp, "Group member\n");
	break;
    default:
	ProTKFprintf(fp, "**UNKNOWN**\n");
	break;
    }

/*--------------------------------------------------------------------*\
    Get the pattern status of the feature
\*--------------------------------------------------------------------*/
    status = ProFeaturePatternStatusGet(feature, &pstatus);
    TEST_CALL_REPORT("ProFeaturePatternStatusGet()", "ProUtilFeatureDump()",
                                        status, status != PRO_TK_NO_ERROR);
    status = ProFeatureGrppatternStatusGet(feature, &gpstatus);
    TEST_CALL_REPORT("ProFeatureGrppatternStatusGet()", "ProUtilFeatureDump()",
					status, status != PRO_TK_NO_ERROR);
    ProTKFprintf(fp, "PATTERN STATUS       ");
    switch(pstatus)
    {
    case PRO_PATTERN_LEADER:
	ProTKFprintf(fp, "Leader of a feature pattern\n");
	break;
    case PRO_PATTERN_MEMBER:
	ProTKFprintf(fp, "Member of a feature pattern\n");
	break;
    default:
	break;
    }
    switch(gpstatus)
    {
    case PRO_GRP_PATTERN_LEADER:
        ProTKFprintf(fp, "Leader of a group pattern\n");
        break;
    case PRO_GRP_PATTERN_MEMBER:
        ProTKFprintf(fp, "Member of a group pattern\n");
        break;
    case PRO_GRP_PATTERN_NONE:
        if (pstatus == PRO_PATTERN_NONE)
            ProTKFprintf(fp, "Not a pattern member\n");
        break;
    default:
        break;
    }

/*--------------------------------------------------------------------*\
    Get elements info. 
\*--------------------------------------------------------------------*/
    ProUtilFeatureElementsDump( feature, fp);

/*--------------------------------------------------------------------*\
    Find whether the feature is an NC sequence
\*--------------------------------------------------------------------*/
    status = ProFeatureIsNcseq(feature, &is_nc_seq);

   if ( is_nc_seq == PRO_B_TRUE )
	ProTKFprintf(fp, "Feature is an NC Sequence\n");
    TEST_CALL_REPORT("ProFeatureIsNcseq()", "ProUtilFeatureDump()",
				PRO_TK_NO_ERROR, status != PRO_TK_NO_ERROR);

/*--------------------------------------------------------------------*\
    Find out whether the feature is created in an assembly
\*--------------------------------------------------------------------*/
    status = ProFeatureSelectionGet(feature, &selection);
    TEST_CALL_REPORT("ProFeatureSelectionGet()", "ProUtilFeatureDump()",
				    status, ((status != PRO_TK_NO_ERROR) &&
					     (status != PRO_TK_BAD_CONTEXT) &&
					     (status != PRO_TK_E_NOT_FOUND)));
    if(!(status == PRO_TK_BAD_CONTEXT))
    if(status == PRO_TK_E_NOT_FOUND)
    {
	ProTKFprintf(fp,"ASSEMBLY REFERENCES :\n");
	ProTKFprintf(fp," (Assembly is not in session)\n");
    }
    else if(status == PRO_TK_NO_ERROR)
    {
	ProTKFprintf(fp,"ASSEMBLY REFERENCES :\n");
	
	status = ProSelectionAsmcomppathGet(selection, &path);
	TEST_CALL_REPORT("ProSelectionAsmcomppathGet()", "ProUtilFeatureDump()",
			 status, status != PRO_TK_NO_ERROR);

	status = ProSelectionModelitemGet(selection, &modelitem);
	TEST_CALL_REPORT("ProSelectionModelitemGet()", "ProUtilFeatureDump()",
			 status, status != PRO_TK_NO_ERROR);
	
/*--------------------------------------------------------------------*\
	Assembly name
\*--------------------------------------------------------------------*/

	ProUtilModelnameGet((ProMdl*)&path.owner, model_name,  model_type);
	ProTKFprintf(fp,"    Owning model : %s.%s\n", model_name, model_type);

/*--------------------------------------------------------------------*\
	Feature id and type
\*--------------------------------------------------------------------*/
	status = ProFeatureTypeGet(&modelitem, &ftype);
	TEST_CALL_REPORT("ProFeatureTypeGet()", "ProUtilFeatureDump()",
					status, status != PRO_TK_NO_ERROR);
	ProUtilFeattypeStr(ftype, ftype_str);
	ProTKFprintf(fp,"    Feature      : %d, type %s\n", modelitem.id, ftype_str);

/*--------------------------------------------------------------------*\
	Component id table
\*--------------------------------------------------------------------*/
	if (path.table_num > 0)
	{
	    ProTKFprintf(fp,"    comp_id_table   ");	    
	    for(m=0; m<path.table_num; m++)
	        ProTKFprintf(fp,"%d", path.comp_id_table[m]);
	    ProTKFprintf(fp, "\n");
	}
	
    }
}



/*====================================================================*\
    FUNCTION :	ProUtilFeatureElementsDump()
    PURPOSE  :	To write a decription of a feature elements to a text file
\*====================================================================*/
void ProUtilFeatureElementsDump(
    ProModelitem *feature,
    FILE *fp)
{
    ProError		status;
    ProElement		elem_tree;
    FeatureElemPrintData print_data;
    ElemtreeElement *elements;
    int elements_num, i;

/*----------------------------------------------*\
    Create a copy of the feature element tree 
\*----------------------------------------------*/
    status = ProFeatureElemtreeExtract( feature, NULL, PRO_FEAT_EXTRACT_NO_OPTS, &elem_tree );
    TEST_CALL_REPORT("ProFeatureElemtreeExtract()", 
	"ProUtilFeatureElementsDump()", status, 
              (status != PRO_TK_NO_ERROR) && (status != PRO_TK_INVALID_TYPE));

/*-----------------------------------------------------------*\
    For non-supported features, don't dump the element info
\*-----------------------------------------------------------*/
    if (status == PRO_TK_INVALID_TYPE)
      {
        ProTKFprintf(fp, "Element tree is not supported for this feature\n");
      }
    else
       if (status == PRO_TK_NO_ERROR)
          {
            print_data.fp = fp;
            print_data.p_feature = feature;
			          
            status = ProUtilCollectElemtreeElements (elem_tree, NULL, &elements);
            if (status == PRO_TK_NO_ERROR)
            { 
                status = ProArraySizeGet ((ProArray)elements, &elements_num);
                TEST_CALL_REPORT( "ProArraySizeGet()",
		    "ProUtilFeatureElementsDump()",
		    status, status != PRO_TK_NO_ERROR );
                for (i = 0; i < elements_num; i++)
                {
                    status = ProUtilElemPrint (elem_tree, elements[i].p_element,
                        elements[i].p_elempath, &print_data);
                }
                status = ProUtilElemtreeElementArrayFree (&elements);
            }
          }

}



/*====================================================================*\
    FUNCTION :	ProUtilElemPrint()
    PURPOSE  :	Visit function.
\*====================================================================*/
ProError ProUtilElemPrint( 
    ProElement elem_tree, 
    ProElement  elem,
    ProElempath elem_path, 
    FeatureElemPrintData* p_data )
{
    ProError		status;
    ProElemId		elem_id;
    int			dimension_id = -1;
    ProBoolean		is_visible;
    ProBoolean		is_incomplete;
    ProBoolean		is_compound;
    ProBoolean          is_array;
    ProBoolean          is_multival;
    ProValue*		p_values;
    ProValue            value;
    int			array_size;
    ProValueData	value_data;
    char		value_data_str[ 48 ];
    int			i;
    ProBoolean          flag = PRO_B_TRUE;             

    status = ProElementIdGet( elem, &elem_id );
    TEST_CALL_REPORT("ProElementIdGet()", 
	"ProUtilElemPrint()",
	PRO_TK_NO_ERROR, status != PRO_TK_NO_ERROR);
    ProTKFprintf( p_data->fp, "Element ID: %d\n", elem_id );

    status = ProFeatureElemIsVisible( p_data->p_feature, elem_path, 
	&is_visible );
#if 0
    TEST_CALL_REPORT("ProFeatureElemIsVisible()",
	"ProUtilElemPrint()", status,
	status != PRO_TK_NO_ERROR );
#else
/* This function return PRO_TK_E_NOT_FOUND for Element FEATURE_TYPE,
from feature PRO_FEAT_DATUM_QUILT (it's a bug)*/
    TEST_CALL_REPORT("ProFeatureElemIsVisible()",
	"ProUtilElemPrint()", status,
        0);
#endif

    status = ProFeatureElemIsIncomplete( p_data->p_feature, elem_path, 
	&is_incomplete );
#if 0
    /* spr 751801 */
    TEST_CALL_REPORT( "ProFeatureElemIsIncomplete()", "ProUtilElemPrint()", 		status, status != PRO_TK_NO_ERROR);
#else
    TEST_CALL_REPORT( "ProFeatureElemIsIncomplete()", "ProUtilElemPrint()",     	status, 0 );
#endif

    status = ProElementIsCompound( elem, NULL, &is_compound );
    TEST_CALL_REPORT("ProElementIsCompound()", "ProUtilElemPrint()",
	status, status != PRO_TK_NO_ERROR);

    status = ProElementIsArray(elem, NULL, &is_array );
    TEST_CALL_REPORT("ProElementIsArray()", "ProUtilElemPrint()",
        status, status != PRO_TK_NO_ERROR);

    ProTKFprintf( p_data->fp, is_visible == PRO_B_TRUE ? "\tVisible\n" : "\tInvisible\n" );
    ProTKFprintf( p_data->fp, is_incomplete == PRO_B_TRUE ? "\tIncomplete\n" : 
	"\tComplete\n" );

    ProTKFprintf( p_data->fp, is_compound == PRO_B_TRUE ? "\tCompound\n" : 
	                            (is_array == PRO_B_TRUE ? "\tArray\n" :
                                           "\tNeither compound nor array\n"));

    if( is_compound == PRO_B_TRUE || is_array == PRO_B_TRUE)
	return PRO_TK_NO_ERROR;

    status = ProFeatureElemDimensionIdGet( p_data->p_feature, elem_path, 
	&dimension_id );
    TEST_CALL_REPORT("ProFeatureElemDimensionIdGet()", 
	"ProUtilElemPrint()",
	status, (status != PRO_TK_NO_ERROR)&&
                (status != PRO_TK_E_NOT_FOUND));

     if (status != PRO_TK_E_NOT_FOUND)
         ProTKFprintf( p_data->fp, "\tDimension ID: %d\n", dimension_id );
     else
         ProTKFprintf( p_data->fp, "\tThis element does not contain dimension\n");

    status = ProArrayAlloc( 0, sizeof(ProValue), 1, (ProArray*)&p_values );
    TEST_CALL_REPORT( "ProArrayAlloc()", "ProUtilElemPrint()",
			status, status != PRO_TK_NO_ERROR );

    status = ProElementIsMultival(elem, NULL, &is_multival);
    TEST_CALL_REPORT( "ProElementIsMultival", 
                       "ProUtilElemPrint", status, status != PRO_TK_NO_ERROR);

    if (is_multival == PRO_B_TRUE)
    {
       status = ProFeatureElemValuesGet( p_data->p_feature,
                                          elem_path, &p_values );
#if 0
	TEST_CALL_REPORT( "ProFeatureElemValuesGet()", "ProUtilElemPrint()",
	    status,
	    status != PRO_TK_NO_ERROR );
#else
/* ProFeatureElemValuesGet() - returns PRO_TK_E_NOT_FOUND in the PIPE mode
for Element PRO_E_STD_EDGE_CHAMF_EDGES.(bugminsk1347)*/
	TEST_CALL_REPORT( "ProFeatureElemValuesGet()", "ProUtilElemPrint()",
	    status,
	    0 );
#endif
    }
    else
    {
       status = ProElementValueGet(elem, &value);
       TEST_CALL_REPORT( "ProElementValueGet", "ProUtilElemPrint",
                                status, status != PRO_TK_NO_ERROR);
  
       status = ProArrayObjectAdd((ProArray*)&p_values, PRO_VALUE_UNUSED,
                                                                1, &value);
       TEST_CALL_REPORT( "ProArrayObjectAdd", "ProUtilElemPrint",
                                status, status != PRO_TK_NO_ERROR);
    }
    if (p_values!=NULL){
        array_size = 0;
        status = ProArraySizeGet( (ProArray)p_values, &array_size );
        TEST_CALL_REPORT( "ProArraySizeGet()", "ProUtilElemPrint()",
			status, status != PRO_TK_NO_ERROR );

        ProTKFprintf( p_data->fp, "\t%s\n",(i=1)?"Value:":"Values:" );
        for( i=0; i<array_size; i++ )
	  {
	   /*To avoid PRO_E_SKETCHER and PRO_E_PATTERN, following condition is set*/
	    if(( elem_id == 458) || (elem_id == 27))
	      flag = PRO_B_FALSE; 
	    
	    if(flag == PRO_B_TRUE)
	      {
		status = ProValueDataGet( p_values[i], &value_data );
        	TEST_CALL_REPORT( "ProValueDataGet()", "ProUtilElemPrint()",
				  status, status != PRO_TK_NO_ERROR );
		switch( value_data.type )
		  {
		  case PRO_VALUE_TYPE_INT:
	            ProTKSprintf( value_data_str, "%d", value_data.v.i );
	            break;
		    
		  case PRO_VALUE_TYPE_DOUBLE:
	            ProTKSprintf( value_data_str, "%f", value_data.v.d );
                    break;
		    
		  case PRO_VALUE_TYPE_POINTER:
	            ProTKSprintf( value_data_str, "Pointer=*", value_data.v.p );
	            break;
		    
		  case PRO_VALUE_TYPE_STRING:
		    
                    if ( value_data.v.s !=NULL)
		      ProTKSprintf( value_data_str, "%s", value_data.v.s );
		    else 
		      ProTKSprintf( value_data_str,"<NULL>" );
		    break;
		    
		  case PRO_VALUE_TYPE_WSTRING:
		    
		    /* NOTE - NULL is possible in some element trees 
		       (for example, for the element PRO_E_STD_FEATURE_NAME
		       where NULL means that feature has the default name) */
		    
                    if ( value_data.v.w !=NULL)
		      ProWstringToString( value_data_str, value_data.v.w );
                    else 
		      ProTKSprintf( value_data_str,"<NULL>" );
	            break;
		    
		  case PRO_VALUE_TYPE_SELECTION:
	            ProTKSprintf( value_data_str, "Selection" );
	            break;
		    
		  case PRO_VALUE_TYPE_TRANSFORM:
                    ProTKSprintf( value_data_str, "Transformation" );
                    break;
		    
		  default:
	            ProTKSprintf( value_data_str, "**UNKNOWN**\n" );
		  }
		
		ProTKFprintf( p_data->fp, "\t%s\n", value_data_str );
	      }
	  }
	
        status = ProArrayFree( (ProArray*)&p_values );
        TEST_CALL_REPORT( "ProArrayFree()", "ProUtilFeatureElementsDump()",
			  status, status != PRO_TK_NO_ERROR );
    }
    
    
    return PRO_TK_NO_ERROR;
}


/*===============================================================*\
FUNCTION : ProUtilFeatCreate
PURPOSE  : Creates feature in no mode option
returns : 
PRO_TK_NO_ERROR - Successfully executed
\*===============================================================*/
ProError ProUtilFeatCreate(ProAsmcomppath *p_comp_path ,ProMdl model, 
                           ProElement pro_e_feature_tree, 
                           ProFeature *feature)
{
    ProError status = PRO_TK_GENERAL_ERROR;
    ProSelection model_sel;
    ProModelitem model_item;
    ProErrorlist p_errors;
    ProFeatureCreateOptions no_mode_option[1];
    int no_opts = 1;
    no_mode_option[0]  = PRO_FEAT_CR_NO_OPTS;
    status = ProMdlToModelitem( model, &model_item ); 
    TEST_CALL_REPORT("ProMdlToModelitem", "ProUtilFeatCreate", 
			status, status != PRO_TK_NO_ERROR );
    status = ProSelectionAlloc (p_comp_path, &model_item, &model_sel);
    status = ProFeatureCreate (model_sel, pro_e_feature_tree, 
       no_mode_option, no_opts, feature, &p_errors); 
    TEST_CALL_REPORT("ProFeatureCreate", "ProUtilFeatCreate", 
			status, status != PRO_TK_NO_ERROR ); 
    ProSelectionFree(&model_sel);
    return status;
}
