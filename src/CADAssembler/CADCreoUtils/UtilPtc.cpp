
#include <ProToolkit.h>
#include <iostream>
#include "UtilPtc.h"
#include <ProUtil.h>

#include "UtilTree.h"	
#include <ProTKRunTime.h>

#include <ProAssembly.h>
#include <ProFeature.h>
#include <ProSolid.h>

/*=============================================================*\
	Function:     ProUtilFeatErrsWrite
	Purpose:      Debug function. Print elem tree and errors if feature
				creation or redefenition failed
*=============================================================*/
void ProUtilFeatErrsWrite(
	char *function,
	ProError err,
	ProElement tree,
	ProErrorlist *err_list)
{
	ProTKFprintf(stderr, "Function %s returned %d\n", function, err);
	ProUtilElemtreePrint(tree, (UtilTreeprintWindow)2, 0);
	ProUtilFeatErrsPrint(err_list);
}

/*=============================================================*\
	Function:     write_feature
	Purpose:      Debug function. Print elem tree and errors if feature
				creation or redefenition failed
*=============================================================*/


ProError write_feature( ProFeature* in_feature, ProError in_status, ProAppData app_data ) {
    ProAsmcomppath* path = NULL;
    ProFeatureElemtreeExtractOptions opts = PRO_FEAT_EXTRACT_NO_OPTS;
    ProElement elem_tree;
	ProError status;
	switch( status = ProFeatureElemtreeExtract(in_feature, path, opts, &elem_tree) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		std::cout << "could not extract feature " << in_feature->type 
			<< " error: " << status << std::endl;
		return PRO_TK_GENERAL_ERROR;
	}
	// ProElemtreeWrite(, PRO_ELEMTREE_XML, );
	ProUtilElemtreePrint(elem_tree, PRO_TEST_INFO_WINDOW, NULL);
	ProFeatureElemtreeFree(in_feature, elem_tree);

	int* parents;
	int parent_count;
	switch( status = ProFeatureParentsGet( in_feature, &parents, &parent_count ) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		std::cout << "could not get parents" << status << std::endl;
		return PRO_TK_GENERAL_ERROR;
	}
	std::cout << "feature id: " << in_feature->id << std::endl;
	for( int ix=0; ix < parent_count; ++ix ) {
		std::cout << " parent id: " << ix << ":" << parents[ix] << std::endl;
	}
	ProArrayFree( (ProArray*)&parents );

	return PRO_TK_NO_ERROR;
}

/**
Prompt the user to select some items and then print their information.
*/
ProError write_selected_feature(void) {
	ProError status;
	ProSelection* selection_array;
	int num_selected;
	switch( status = ProSelect((char *)"feature", 1, NULL, NULL, NULL, NULL, 
		&selection_array, &num_selected) ) {
		case PRO_TK_NO_ERROR: break;
		default:
		return PRO_TK_GENERAL_ERROR; 
	}
	for( int ix=0; ix < num_selected; ++ix ) {
		ProFeature feature;
		switch( status = ProSelectionModelitemGet(selection_array[ix], &feature) ) {
			case PRO_TK_NO_ERROR: break;
			default:
			continue;
		}

		write_feature( &feature, PRO_TK_NO_ERROR, NULL );
	} 
	return PRO_TK_NO_ERROR;
}

ProError write_selected_feature_as_xml( char* in_output_file ) {
	ProError status;
	ProSelection* selection_array;
	int num_selected;
	switch( status = ProSelect((char *)"feature", 1, NULL, NULL, NULL, NULL, 
		&selection_array, &num_selected) ) {
		case PRO_TK_NO_ERROR: break;
		default:
		return PRO_TK_GENERAL_ERROR; 
	}
	for( int ix=0; ix < num_selected; ++ix ) {
		ProFeature feature;
		switch( status = ProSelectionModelitemGet(selection_array[ix], &feature) ) {
		case PRO_TK_NO_ERROR: break;
		default:
			std::cout << "could not get feature" << std::endl;
			continue;
		}
		ProAsmcomppath* path = NULL;
        ProFeatureElemtreeExtractOptions opts = PRO_FEAT_EXTRACT_NO_OPTS;
  		ProElement         element_tree;
		ProError stat;
		switch( stat=ProFeatureElemtreeExtract( &feature, path, opts, &element_tree) ) {
		case PRO_TK_NO_ERROR: break;
		default:
			std::cout << "could not extract tree from feature" << std::endl;
			continue;
		}
		ProName file_name;
		ProStringToWstring( file_name, in_output_file );
		ProName extension;
		ProStringToWstring( extension, "xpt" );
        ProPath output_file; 
		ProPathCreate(NULL, file_name, extension, PRO_VALUE_UNUSED, output_file);

		switch( stat = ProElemtreeWrite(element_tree, PRO_ELEMTREE_XML, output_file) ) {
		case PRO_TK_NO_ERROR: break;
		default:
			std::cout << "could not write element tree of feature" << std::endl;
			continue;
		}
	} 
	return PRO_TK_NO_ERROR;
}


ProError write_all_feature(const std::string feature_name) {
		
	ProMdl total_model;
	switch( ProMdlActiveGet(&total_model) ) {
	case PRO_TK_NO_ERROR: break;
	default:  
		std::cout << "no currently active model" << std::endl;
		return PRO_TK_GENERAL_ERROR; 
	} 
	ProSolid total_solid = static_cast<ProSolid>(total_model);

    ProFeatureFilterAction  filter_action = NULL;
    ProAppData   app_data = NULL;
	switch( ProSolidFeatVisit(total_solid, write_feature, filter_action, app_data) ) {
	case PRO_TK_NO_ERROR: break;
	default:  
		std::cout << "no currently active model" << std::endl;
		return PRO_TK_GENERAL_ERROR; 
	} 
	return PRO_TK_NO_ERROR;
}