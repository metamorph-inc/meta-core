
#ifndef UTILFEATS_H
#define UTILFEATS_H

/*--------------------------------------------------------------------*\
Application macros
\*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*\
Application data types
\*--------------------------------------------------------------------*/
typedef struct tag_FeatureElemPrintData
	{
	    FILE*	    fp;
	    ProFeature*	    p_feature;
	} FeatureElemPrintData;


/*--------------------------------------------------------------------*\
Application external data
\*--------------------------------------------------------------------*/

extern void ProUtilFeatureDump( ProModelitem	*feature,
			       FILE		*fp);
/*
    Purpose:	Write a desription of a feature to a text file

    Input Arguments:
	feature		- the feature
	fp		- pointer to the text file (assumed to be open
							for write)
*/

extern void ProUtilFeatureElementsDump( ProModelitem *feature, FILE *fp);

/*
    Purpose:	Write a desription of a feature elements to a text file

    Input Arguments:
	feature		- the feature
	fp		- pointer to the text file (assumed to be open
							for write)
*/

extern ProError ProUtilElemPrint( 
    ProElement elem_tree, 
    ProElement  elem,
    ProElempath elem_path, 
    FeatureElemPrintData* p_data );

/*
    Purpose:	Visit function.

*/


ProError ProUtilFeatCreate(ProAsmcomppath *p_comp_path ,ProMdl model, 
                           ProElement pro_e_feature_tree, 
                           ProFeature *feature);
                           

#endif /* ifdef UTILFEATS_H */
