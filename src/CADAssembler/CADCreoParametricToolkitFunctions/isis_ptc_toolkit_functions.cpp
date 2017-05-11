#include <isis_ptc_toolkit_functions.h>
#include <CADCommonConstants.h>
#include <CreoErrorCodes.h>
#include <MultiFormatString.h>
#include <CreoStringToEnumConversions.h>
#include "LoggerBoost.h"
#include "CommonDefinitions.h"
#include <UtilCollect.h>

// extern "C" FILE* PTApplsUnicodeFopen(const char *filename, const char *mode);

namespace isis
{

	const int ERROR_STRING_BUFFER_LENGTH = 1024;

	void GetModelNameForErrorMessages( ProMdl     handle,
									   char       name_narrow[])
	{
		name_narrow[0] = '\0';
		// don't let this function throw an exception.
		try
		{
			ProName    name_wide;
			isis_ProMdlNameGet(handle, name_wide);
			ProWstringToString(name_narrow,name_wide);
		}
		catch (...)
		{
		}
	}


    ProError isis_ProEngineerStart(	char* proe_path,
									char* prodev_text_path )
									throw (isis::application_exception)
	{
		ProError err = ProEngineerStart( proe_path, prodev_text_path );
		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProEngineerStart returned ProError: %s(%d), proe_path: %s,  prodev_text_path: %s", ProToolKitError_string(err).c_str(), err, proe_path, prodev_text_path );
			throw isis::application_exception("C06001",err_str); 
		}

		return err;
	}


	ProError isis_ProMdlRetrieve( const ProFamilyName name, 
								  ProMdlType    type,
								  ProMdl       *p_handle)
								  throw (isis::application_exception)
	{
		ProError err = ProMdlRetrieve( (wchar_t *)name, type, p_handle);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			//std::string type_string;
			//if ( type == PRO_MDL_PART )
			//	type_string = "Part";
			//else
			//	type_string = "Assembly";


			char name_narrow[ISIS_CHAR_BUFFER_LENGTH];
			ProWstringToString(name_narrow, (wchar_t *)name); 
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			const char* extra = "";
			if (err == PRO_TK_E_NOT_FOUND)
			{
				extra = ", Possible causes: 1. No model by this name on load path 2. wrong type (asm vs prt) 3. a model created with Academic Creo being loaded by Commercial Creo, or vice versa";
			}
			sprintf( err_str, "exception : ProMdlRetrieve returned ProError: %s(%d), Model Name: %s  Type: %s%s", ProToolKitError_string(err).c_str(), err, name_narrow, isis::ProMdlType_string(type).c_str(), extra );
			throw isis::application_exception("C06002",err_str); 
		}

		return err;

	}


	ProError isis_ProMdlDisplay( ProMdl    handle )
								  throw (isis::application_exception)
	{
		ProError err = ProMdlDisplay( handle);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMdlDisplay returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06003",err_str); 
		}

		return err;
	}

	ProError isis_ProWindowRefresh( int window_id )
								  throw (isis::application_exception)
	{
		ProError err = ProWindowRefresh( window_id);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProWindowRefresh returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06004",err_str); 
		}

		return err;
	}

	ProError isis_ProWindowRepaint ( int window_id )
								  throw (isis::application_exception)
	{
		ProError err = ProWindowRepaint ( window_id);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProWindowRepaint  returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06005",err_str); 
		}

		return err;
	}


	ProError isis_ProMdlWindowGet( ProMdl mdl, int* window_id )
								  throw (isis::application_exception)
	{
		ProError err = ProMdlWindowGet( mdl,
										window_id);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMdlWindowGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06006",err_str); 
		}

		return err;
	}

	ProError isis_ProWindowCurrentSet(int window_id )
								  throw (isis::application_exception)
	{
		ProError err = ProWindowCurrentSet( window_id );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProWindowCurrentSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06007",err_str); 
		}

		return err;
	}

	ProError isis_ProWindowActivate(int window_id )
								  throw (isis::application_exception)
	{
		ProError err = ProWindowActivate( window_id );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProWindowActivate returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06008",err_str); 
		}

		return err;
	}



	ProError isis_ProMdlDelete( ProMdl    handle )
								  throw (isis::application_exception)
	{
		ProError err = ProMdlDelete( handle);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMdlDelete returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06009",err_str); 
		}

		return err;
	}


	ProError isis_ProMdlEraseNotDisplayed(  )
								  throw (isis::application_exception)
	{
		ProError err = ProMdlEraseNotDisplayed();

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMdlEraseNotDisplayed returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06010",err_str); 
		}

		return err;
	}


	extern ProError isis_ProSolidCreate  (	const ProName     name, 
											ProType     type,
											ProSolid   *p_handle)
											throw (isis::application_exception)
	{
		ProError err = ProSolidCreate( (wchar_t *) name, type, p_handle);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSolidCreate returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06011",err_str); 
		}

		return err;
	}




	ProError isis_ProEngineerEnd( void ) throw (isis::application_exception)
	{
		ProError err = ProEngineerEnd();

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProEngineerEnd returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06012",err_str); 
		}
		return err;
	}

	/*
	FILE *isis_PTApplsUnicodeFopen(const char *filename, const char *mode) throw(isis::application_exception)
	{
		FILE* fp = PTApplsUnicodeFopen( filename, mode);

		if ( fp == NULL ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : PTApplsUnicodeFopen returned Null" );
			throw isis::application_exception("C06013",err_str); 
		}
		
		return fp;
	}
	*/


	ProError isis_ProMdlDataGet( ProMdl handle, ProMdldata *p_data) throw(isis::application_exception)
	{
		ProError err = ProMdlDataGet( handle, p_data);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMdlDataGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06014",err_str); 
		}
		return err;
	}


	ProError isis_ProSolidFeatVisit ( ProSolid     p_handle, 
                                 ProFeatureVisitAction  visit_action,
                                 ProFeatureFilterAction  filter_action,
                                 ProAppData   app_data) throw(isis::application_exception)
	{

		ProError err = ProSolidFeatVisit ( p_handle, visit_action,filter_action,app_data  );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSolidFeatVisit returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06015",err_str); 
		}
		return err;
	}

	ProError isis_ProAsmcompMdlGet ( ProAsmcomp *p_feat_handle,
									 ProMdl     *p_mdl_handle) throw(isis::application_exception)
	{

		ProError err = ProAsmcompMdlGet ( p_feat_handle, p_mdl_handle);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAsmcompMdlGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06016",err_str); 
		}
		return err;
	}



	ProError isis_ProAsmcomppathMdlGet( ProAsmcomppath *p_path,
										ProMdl         *p_model) throw(isis::application_exception)
	{

		ProError err = ProAsmcomppathMdlGet ( p_path, p_model);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAsmcomppathMdlGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06017",err_str); 
		}
		return err;
	}




	ProError isis_ProFeatureTypeGet  (ProFeature  *p_feat_handle, ProFeattype *p_type) throw(isis::application_exception)
	{
		ProError err = ProFeatureTypeGet ( p_feat_handle, p_type);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProFeatureTypeGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06018",err_str);  
		}
		return err;
	}

	ProError isis_ProAsmcompAssemble ( ProAssembly   owner_assembly,
				     ProSolid           component_model,
				     ProMatrix           init_pos,
				     ProAsmcomp *feature ) throw(isis::application_exception)
	{
		ProError err = ProAsmcompAssemble ( owner_assembly, component_model,init_pos, feature );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAsmcompAssemble returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06019",err_str);  
		}
		return err;

	}

	ProError isis_ProAsmcompCreateCopy ( ProAssembly  assembly,
				      ProName      comp_name,
				      ProMdlType   comp_type,
				      ProMdl       template_model,
				      ProBoolean   leave_unplaced,
				      ProAsmcomp  *p_comp_handle) throw(isis::application_exception)
	{
		ProError err = ProAsmcompCreateCopy (	assembly,
												comp_name,
												comp_type,
												template_model,
												leave_unplaced,
												p_comp_handle );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAsmcompCreateCopy returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06019",err_str);  
		}
		return err;
	}

	ProError isis_ProAsmcompFillFromMdl ( ProAsmcomp *comp_handle,
										  ProMdl      template_model) throw(isis::application_exception)
	{

		ProError err = ProAsmcompFillFromMdl(	comp_handle, template_model );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAsmcompFillFromMdl returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06019",err_str);  
		}
		return err;
	}


	ProError isis_ProMdlSave  (ProMdl    handle) throw(isis::application_exception)
	{
		ProError err = ProMdlSave ( handle );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMdlSave returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06020",err_str);  
		}
		return err;

	}


	ProError isis_ProMdlErase  (ProMdl    handle) throw(isis::application_exception)
	{
		ProError err = ProMdlErase ( handle );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMdlErase returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06021",err_str);  
		}
		return err;

	}



	ProError isis_ProMdlfileCopy (	ProMdlType   mdl_type,
									const ProName      from_name,
									const ProName      to_name )
	{

		ProError err = ProMdlfileCopy (  mdl_type, (wchar_t *)from_name,  (wchar_t *)to_name);

		if ( err != PRO_TK_NO_ERROR ) 
		{

			char from_name_narrow[ISIS_CHAR_BUFFER_LENGTH];
			char to_name_narrow[ISIS_CHAR_BUFFER_LENGTH];
			ProWstringToString(from_name_narrow,(wchar_t *)from_name);
			ProWstringToString(to_name_narrow,(wchar_t *)to_name);
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMdlfileCopy returned ProError: %s(%d), From Name: %s, To Name: %s", ProToolKitError_string(err).c_str(), err, from_name_narrow, to_name_narrow );
			throw isis::application_exception("C06022",err_str);  
		}
		return err;
	}

	ProError isis_ProModelitemByNameInit( ProMdl         mdl, 
                                         ProType        type, 
                                         const ProName        name, 
                                         ProModelitem*  p_item ) throw(isis::application_exception)
	{
		ProError err = ProModelitemByNameInit ( mdl, type, (wchar_t *)name, p_item );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  model_name_narrow[PRO_NAME_SIZE];
			GetModelNameForErrorMessages( mdl, model_name_narrow );
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			char name_narrow[PRO_NAME_SIZE];
			ProWstringToString(name_narrow,(wchar_t *)name); 
			sprintf( err_str, "exception : ProModelitemByNameInit returned ProError: %s(%d), Model Name: %s, Feature Name: %s", ProToolKitError_string(err).c_str(), err, model_name_narrow, name_narrow);
			throw isis::application_exception("C06023",err_str);  
		}
		return err;
	}

	ProError isis_ProModelitemNameGet ( ProModelitem *p_item,
										ProName       name ) throw(isis::application_exception)
	{
		ProError err = ProModelitemNameGet ( p_item, name );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : isis_ProModelitemNameGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06024",err_str);  
		}
		return err;
	}


	ProError isis_ProFeatureInit ( ProSolid     p_owner_handle, 
                                  int          feat_id, 
                                  ProFeature  *p_feat_handle ) throw(isis::application_exception)
	{
		ProError err = ProFeatureInit ( p_owner_handle, feat_id, p_feat_handle );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProFeatureInit returned ProError: %s(%d), Feature ID: %d", ProToolKitError_string(err).c_str(),err, feat_id);
			throw isis::application_exception("C06025",err_str);  
		}
		return err;

	}


	ProError isis_ProFeatureElemValueGet (	ProFeature    *feature,
											ProElempath    elempath,
											ProValue      *p_elemvalue ) throw(isis::application_exception)
	{
		ProError err = ProFeatureElemValueGet ( feature, elempath, p_elemvalue );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProFeatureElemValueGet returned ProError: %s(%d), Feature ID: %d",ProToolKitError_string(err).c_str(), err, feature->id );
			throw isis::application_exception("C06026",err_str);
		}
		return err;
	}

	ProError isis_ProElempathAlloc ( ProElempath *p_elem_path  ) throw(isis::application_exception)
	{
		ProError err = ProElempathAlloc ( p_elem_path );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProElempathAlloc returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06027",err_str);  
		}
		return err;

	}

	ProError isis_ProElempathFree ( ProElempath *p_elem_path  ) throw(isis::application_exception)
	{
		ProError err = ProElempathFree ( p_elem_path );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProElempathFree returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06027",err_str);  
		}
		return err;

	}


	ProError isis_ProElempathDataSet ( ProElempath       elem_path,
                                     ProElempathItem  *elempath_data,
                                     int               path_size  ) throw(isis::application_exception)
	{
		ProError err = ProElempathDataSet ( elem_path, elempath_data, path_size );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProElempathDataSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06028",err_str);  
		}
		return err;

	}
	

	ProError isis_ProValueDataGet ( ProValue       value,
									ProValueData  *value_data ) throw(isis::application_exception)
	{
		ProError err = ProValueDataGet ( value, value_data );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProValueDataGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06029",err_str);  
		}
		return err;

	}


	ProError isis_ProElementArrayCount (ProElement    elemtree,
                                      ProElempath   elempath,
                                      int          *p_array_count) throw(isis::application_exception)
	{
		ProError err = ProElementArrayCount ( elemtree, elempath, p_array_count );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProElementArrayCount returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06030",err_str);  
		}
		return err;
	}

	ProError isis_ProFeatureElemtreeExtract (	ProFeature *feature,
												ProAsmcomppath *path,
												ProFeatureElemtreeExtractOptions opts,
												ProElement *p_elem) throw(isis::application_exception)
	{
		ProError err = ProFeatureElemtreeExtract ( feature, path, opts, p_elem  );
		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProFeatureElemtreeExtract returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06031",err_str);  
		}
		return err;
	}

	ProError isis_ProFeatureElemtreeFree (	ProFeature *feature,
											ProElement  p_elem) throw(isis::application_exception)
	{
		ProError err = ProFeatureElemtreeFree (feature, p_elem  );
		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProFeatureElemtreeFree returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06031",err_str);  
		}
		return err;
	}


	ProError isis_ProFeatureRedefine (	ProAsmcomppath   *comp_path,
										ProFeature       *feature,
										ProElement        elemtree,
										ProFeatureCreateOptions options[],
										int               num_opts,
										ProErrorlist     *p_errors ) throw(isis::application_exception)
	{
		ProError err = ProFeatureRedefine ( comp_path, feature, elemtree, options, num_opts,p_errors);
		if ( err != PRO_TK_NO_ERROR ) 
		{
			isis_LOG(lg, isis_FILE, isis_ERROR) << "p_errors->error_number: " << p_errors->error_number;
			for (int i = 0; i < p_errors->error_number; ++i )
			{
				isis_LOG(lg, isis_FILE, isis_ERROR) << "p_errors->error_list[i].error: "<< p_errors->error_list[i].error;
				isis_LOG(lg, isis_FILE, isis_ERROR) << "p_errors->error_list[i].err_item_id: "<< p_errors->error_list[i].err_item_id;
				isis_LOG(lg, isis_FILE, isis_ERROR) << "p_errors->error_list[i].err_item_type: "<< p_errors->error_list[i].err_item_type;
			}
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProFeatureRedefine returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06031",err_str);  
		}
		return err;
	}


	ProError isis_ProGeomitemFeatureGet (	ProGeomitem  *p_geom_item, 
											ProFeature   *p_feat_handle  ) throw(isis::application_exception)
	{
		ProError err = ProGeomitemFeatureGet ( p_geom_item, p_feat_handle );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProGeomitemFeatureGet returned ProError: %s(%d), Feature ID: %d", ProToolKitError_string(err).c_str(), err, p_geom_item->id);
			throw isis::application_exception("C06032",err_str);  
		}
		return err;

	}

	ProError isis_ProMdlCurrentGet ( ProMdl     *p_handle) throw(isis::application_exception)
	{
		ProError err = ProMdlCurrentGet ( p_handle );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : isis_ProMdlCurrentGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06033",err_str);  
		}
		return err;

	}

	ProError isis_ProModelitemInit ( ProMdl         p_owner_handle, 
									 int            item_id, 
									 ProType        item_type, 
									 ProModelitem  *p_handle) throw(isis::application_exception)
	{
		ProError err = ProModelitemInit ( p_owner_handle, item_id, item_type, p_handle );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProModelitemInit returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06034",err_str);  
		}
		return err;

	}

	ProError isis_ProMdlToModelitem (	ProMdl mdl, 
										ProModelitem* p_model_item ) throw(isis::application_exception)
	{
		ProError err = ProMdlToModelitem ( mdl, p_model_item );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMdlToModelitem returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06035",err_str);  
		}
		return err;
	}

	ProError isis_ProArrayAlloc ( int n_objs,
                                  int obj_size,
                                  int reallocation_size,
                                  ProArray* p_array) throw(isis::application_exception)
	{
		ProError err = ProArrayAlloc ( n_objs, obj_size, reallocation_size, p_array );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProArrayAlloc returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06036",err_str);  
		}
		return err;
	}


	ProError isis_ProArrayFree( ProArray* p_array) throw(isis::application_exception)
	{
		ProError err = ProArrayFree( p_array );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProArrayFreereturned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06037",err_str);  
		}
		return err;
	}

	ProError isis_ProArraySizeGet( ProArray in_array,
									int*     p_size) throw(isis::application_exception)
	{
		ProError err = ProArraySizeGet( in_array, p_size );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProArraySizeGet ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06038",err_str);  
		}
		return err;
	}

	ProError isis_ProAsmcomppathInit (	ProSolid       p_solid_handle,
										ProIdTable     memb_id_tab,
										int            table_size,
										ProAsmcomppath *p_handle) throw(isis::application_exception)
	{
		ProError err = ProAsmcomppathInit ( p_solid_handle, memb_id_tab, table_size, p_handle );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAsmcomppathInit returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06039",err_str);  
		}
		return err;
	}


	ProError isis_ProSelectionAlloc (	ProAsmcomppath *p_cmp_path,
										ProModelitem *p_mdl_itm,
										ProSelection *p_selection ) throw(isis::application_exception)
	{
		ProError err = ProSelectionAlloc ( p_cmp_path, p_mdl_itm, p_selection );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			std::stringstream errorString;
			errorString <<  "exception : ProSelectionAlloc returned ProError: " << ProToolKitError_string(err).c_str() << "(" << err << ")";
			errorString <<  std::endl << "Path: ";
			errorString <<  std::endl << "   Path (Feature IDs): ";
			errorString <<  std::endl << "      Number of Feature IDs in the path: " << p_cmp_path->table_num;

			if ( p_cmp_path->table_num > -1 &&  p_cmp_path->table_num <= PRO_MAX_ASSEM_LEVEL )
				for ( int i = 0; i < p_cmp_path->table_num; ++i ) errorString << std::endl << "      Path Feature ID: " <<  p_cmp_path->comp_id_table[i];
			else
				errorString << std::endl << "   Number of feature IDs in the path is not in the range of 0 to " << PRO_MAX_ASSEM_LEVEL << ".";

			errorString <<  std::endl << "   Path Model Owner: " << p_cmp_path->owner;
			errorString <<  std::endl << "Model Item: ";
			errorString <<  std::endl << "   Model Item Owner:         " << p_mdl_itm->owner;
			errorString <<  std::endl << "   Model Item Feature ID:    " << p_mdl_itm->id;
			errorString <<  std::endl << "   Model Item Type (1-Assembly, 2-Part, 5-Surface, 21-Axis, 66-Point): " << p_mdl_itm->type;

			//char  err_str[ERROR_STRING_BUFFER_LENGTH];
			//sprintf( err_str, "exception : ProSelectionAlloc returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06040",errorString.str().c_str());  
		}
		return err;
	}


	ProError isis_ProSelectionCopy( ProSelection  from_selection, ProSelection  *p_to_selection ) throw(isis::application_exception)
	{
		ProError err =  ProSelectionCopy( from_selection,  p_to_selection );
		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSelectionCopy returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06041",err_str);  
		}
		return err;
	}


	ProError isis_ProFeatureCreate (	ProSelection     model,
										ProElement       elemtree,
										ProFeatureCreateOptions options[],
										int              num_opts,
										ProFeature      *p_feature,
										ProErrorlist    *p_errors ) throw(isis::application_exception)
	{
		ProError err = ProFeatureCreate ( model, elemtree, options, num_opts, p_feature,  p_errors );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			isis_LOG(lg, isis_FILE, isis_ERROR) << "p_errors->error_number: " << p_errors->error_number;
			for (int i = 0; i < p_errors->error_number; ++i )
			{
				isis_LOG(lg, isis_FILE, isis_ERROR) << "p_errors->error_list[i].error: "<< p_errors->error_list[i].error;
				isis_LOG(lg, isis_FILE, isis_ERROR) << "p_errors->error_list[i].err_item_id: "<< p_errors->error_list[i].err_item_id;
				isis_LOG(lg, isis_FILE, isis_ERROR) << "p_errors->error_list[i].err_item_type: "<< p_errors->error_list[i].err_item_type;
			}

			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProFeatureCreate returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06041",err_str);  
		}
		return err;
	}


	ProError isis_ProSelectionFree ( ProSelection *p_selection) 
											throw(isis::application_exception)
	{
		ProError err = ProSelectionFree (p_selection );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSelectionFree returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06042",err_str);  
		}
		return err;
	}


	ProError isis_ProAsmcompconstraintAlloc (	ProAsmcompconstraint *p_constraint) throw(isis::application_exception)
	{
		ProError err = ProAsmcompconstraintAlloc ( p_constraint);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAsmcompconstraintAlloc returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06043",err_str);  
		}
		return err;
	}

	ProError isis_ProAsmcompconstraintFree (	ProAsmcompconstraint constraint ) throw(isis::application_exception)
	{
		ProError err = ProAsmcompconstraintFree ( constraint);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAsmcompconstraintFree returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06044",err_str);  
		}
		return err;
	}

	ProError isis_ProAsmcompConstrRemove (	ProAsmcomp *p_feat_handle,
						    int         index ) throw(isis::application_exception)
	{
		ProError err = ProAsmcompConstrRemove ( p_feat_handle, index);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAsmcompConstrRemove returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06044",err_str);  
		}
		return err;
	}

	ProError isis_ProAsmcompconstraintTypeSet (	ProAsmcompconstraint constraint,
												ProAsmcompConstrType type) throw(isis::application_exception)
	{
		ProError err = ProAsmcompconstraintTypeSet ( constraint, type);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAsmcompconstraintTypeSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06045",err_str);  
		}
		return err;
	}


	ProError isis_ProAsmcompconstraintAsmreferenceSet (	ProAsmcompconstraint constraint,
													ProSelection asm_ref,
													ProDatumside asm_orient) throw(isis::application_exception)
	{
		ProError err = ProAsmcompconstraintAsmreferenceSet ( constraint, asm_ref, asm_orient );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAsmcompconstraintAsmreferenceSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06046",err_str);  
		}
		return err;
	}


	ProError isis_ProAsmcompconstraintAttributesSet (	 ProAsmcompconstraint constraint ,
														int attributes) throw(isis::application_exception)
	{
		ProError err = ProAsmcompconstraintAttributesSet ( constraint, attributes );
		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAsmcompconstraintAttributesSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06046",err_str);  
		}
		return err;
	}
	

	ProError isis_ProAsmcompconstraintCompreferenceSet (	ProAsmcompconstraint constraint,
													ProSelection comp_ref,
													ProDatumside comp_orient) throw(isis::application_exception)
	{
		ProError err = ProAsmcompconstraintCompreferenceSet ( constraint, comp_ref, comp_orient );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAsmcompconstraintCompreferenceSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06047",err_str);  
		}
		return err;
	}


	ProError isis_ProAsmcompRegenerate (	ProAsmcomp *p_comp_handle,
											ProBoolean  update_soft) 
														throw(isis::application_exception)
	{
		ProError err = ProAsmcompRegenerate ( p_comp_handle, update_soft );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAsmcompRegenerate returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06048",err_str);  
		}
		return err;
	}

	ProError isis_ProAsmcompPositionGet (	ProAsmcomp*     component,
											ProMatrix    position) 
														throw(isis::application_exception)
	{
		ProError err = ProAsmcompPositionGet ( component, position );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAsmcompPositionGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06048",err_str);  
		}
		return err;
	}

	ProError isis_ProArrayObjectAdd (	ProArray* p_array,
										int       index,
										int       n_objects,
										void*     p_object) throw(isis::application_exception)
	{
		ProError err = ProArrayObjectAdd ( p_array, index, n_objects, p_object );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProArrayObjectAdd returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06049",err_str);  
		}
		return err;
	}

	
	ProError isis_ProAsmcompConstraintsSet(	ProAsmcomppath* component_path, 
											ProAsmcomp *component,
											ProAsmcompconstraint *p_constraints ) throw(isis::application_exception)
	{
		ProError err = ProAsmcompConstraintsSet ( component_path, component, p_constraints );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAsmcompConstraintsSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06050",err_str);  
		}
		return err;
	}


	ProError isis_ProSolidRegenerate(	ProSolid    p_handle,
                                                  int  flags ) throw(isis::application_exception)
	{
		ProError err = ProSolidRegenerate ( p_handle, flags );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSolidRegenerate returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06051",err_str);  
		}
		return err;
	}


	ProError isis_ProSolidRegenerationstatusGet( ProSolid                    solid,
												 ProSolidRegenerationStatus* regen_status ) 
																throw(isis::application_exception)
	{
		ProError err = ProSolidRegenerationstatusGet ( solid, regen_status );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSolidRegenerationstatusGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06052",err_str);  
		}
		return err;
	}


	ProError isis_ProAsmcompconstraintOffsetSet(	ProAsmcompconstraint constraint,
													double offset) throw(isis::application_exception)
	{
		ProError err = ProAsmcompconstraintOffsetSet ( constraint, offset );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAsmcompconstraintOffsetSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06053",err_str);  
		}
		return err;
	}


	ProError isis_ProUnitsystemTypeGet( ProUnitsystem*     system,
                                        ProUnitsystemType* type)
												throw(isis::application_exception)
	{

		ProError err = ProUnitsystemTypeGet ( system,
											  type );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProUnitsystemTypeGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06054",err_str);  
		}
		return err;
	}


	ProError isis_ProMdlPrincipalunitsystemSet( ProMdl             mdl,
												ProUnitsystem*     new_system,
												ProUnitConvertType type,
												ProBoolean         ignore_param_units,
												int                regeneration_flags)
												throw(isis::application_exception)
	{

		ProError err = ProMdlPrincipalunitsystemSet (	mdl,
														new_system,
														type,
														ignore_param_units,
														regeneration_flags);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMdlPrincipalunitsystemSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06055",err_str);  
		}
		return err;
	}


	ProError isis_ProElementAlloc ( ProElemId name_id,
									ProElement *p_elem)
												throw(isis::application_exception)
	{

		ProError err = ProElementAlloc  ( name_id,
										  p_elem );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProElementAlloc  returned ProError: %s(%d), ProElemId name_id: %d", ProToolKitError_string(err).c_str(),err, name_id );
			throw isis::application_exception("C06056",err_str);  
		}
		return err;
	}

	ProError isis_ProElementValueSet (	ProElement  elem,
										ProValue    value)
												throw(isis::application_exception)
	{

		ProError err = ProElementValueSet  ( elem,
											value );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProElementValueSet  returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06057",err_str);  
		}
		return err;
	}

	ProError isis_ProElementValueGet (	ProElement       elem,
										ProValue        *p_value)
												throw(isis::application_exception)
	{

		ProError err = ProElementValueGet  ( elem,
											p_value );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProElementValueGet  returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06057",err_str);  
		}
		return err;
	}

	ProError isis_ProElementReferenceGet (	ProElement                 element,
                                         ProElementReferenceOptions options,
                                         ProReference*              reference)
												throw(isis::application_exception)
	{

		ProError err = ProElementReferenceGet  ( element, options, reference );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProElementReferenceGet  returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06057",err_str);  
		}
		return err;
	}


	ProError isis_ProElementIntegerSet(	ProElement element, int value)
												throw(isis::application_exception)
	{

		ProError err = ProElementIntegerSet ( element, value );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProElementIntegerSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06058",err_str);  
		}

		return err;
	}



	ProError isis_ProElementReferencesSet(	ProElement    element,
											ProReference* references)
												throw(isis::application_exception)
	{

		ProError err = ProElementReferencesSet ( element, references );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProElementReferencesSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06059",err_str);  
		}
		return err;
	}


	ProError isis_ProElemtreeElementAdd( ProElement     elemtree,
                                         ProElempath    elempath,
                                         ProElement     elem )
												throw(isis::application_exception)
	{

		ProError err = ProElemtreeElementAdd ( elemtree, elempath, elem );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProElemtreeElementAdd returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06060",err_str);  
		}
		return err;
	}


	ProError isis_ProElemtreeElementGet(	ProElement   elemtree,
											ProElempath  elempath,
											ProElement  *p_elem)
												throw(isis::application_exception)
	{


		ProError err = ProElemtreeElementGet ( elemtree, elempath, p_elem );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProElemtreeElementGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06060",err_str);  
		}
		return err;
	}

	ProError isis_ProElementIntegerGet(	ProElement               element, 
                                      ProElementIntegerOptions options, 
                                      int*                     value)
												throw(isis::application_exception)
	{


		ProError err = ProElementIntegerGet ( element, options, value );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProElementIntegerGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06060",err_str);  
		}
		return err;
	}

	ProError isis_ProElementDoubleGet(	ProElement              element, 
										ProElementDoubleOptions options, 
										double*                 value)
												throw(isis::application_exception)
	{


		ProError err = ProElementDoubleGet ( element, options, value );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProElementDoubleGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06060",err_str);  
		}
		return err;
	}

	void isis_AddElementtoElemTree(ProElemId id, ProElement *parent, ProValueData *elem_data, ProElement *elem )
																								throw(isis::application_exception)
	{
	   ProValue value ;

	   if ( elem_data == NULL || elem == NULL || parent == NULL )
	   {
		 	char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : AddElementtoElemTree received bad inputs (elem_data, elem, or parent were NULL");
			throw isis::application_exception("C06060",err_str);  
	   }

	   isis::isis_ProElementAlloc(id, elem) ;

	   isis::isis_ProValueAlloc (&value) ;

		isis::isis_ProValueDataSet (value, elem_data) ;
		isis::isis_ProElementValueSet (*elem, value) ;

	   isis::isis_ProElemtreeElementAdd( *parent, NULL, *elem ) ;
	}


	ProError isis_ProValueAlloc( ProValue *p_value )
										throw(isis::application_exception)
	{

		ProError err = ProValueAlloc ( p_value );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProValueAlloc returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06061",err_str);  
		}
		return err;
	}


	ProError isis_ProValueFree( ProValue value )
										throw(isis::application_exception)
	{

		ProError err = ProValueFree ( value );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProValueFree returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06062",err_str);  
		}
		return err;
	}


	ProError isis_ProValueDataSet(	ProValue       value,
									ProValueData  *value_data )
										throw(isis::application_exception)
	{

		ProError err = ProValueDataSet ( value, value_data );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProValueDataSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06063",err_str);  
		}
		return err;
	}


	ProError isis_ProElementArrayGet(	ProElement      elemtree,
                                    ProElempath     elempath,
                                    ProElement    **p_array_elems )
										throw(isis::application_exception)
	{

		ProError err = ProElementArrayGet ( elemtree, elempath,  p_array_elems);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProElementArrayGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06063",err_str);  
		}
		return err;
	}

	ProError isis_ProFeatureWithoptionsCreate(	ProSelection             model,
                                             ProElement               elemtree,
                                             ProFeatureCreateOptions *options,
                                             int                      flags,
                                             ProFeature              *p_feature,
                                             ProErrorlist            *p_errors)
										throw(isis::application_exception)
	{

		ProError err = ProFeatureWithoptionsCreate ( model, elemtree, options, flags, p_feature,  p_errors );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			
			isis_LOG(lg, isis_FILE, isis_ERROR) << "p_errors->error_number: " << p_errors->error_number;
			for (int i = 0; i < p_errors->error_number; ++i )
			{
				isis_LOG(lg, isis_FILE, isis_ERROR) << "p_errors->error_list[i].error: "<< p_errors->error_list[i].error;
				isis_LOG(lg, isis_FILE, isis_ERROR) << "p_errors->error_list[i].err_item_id: "<< p_errors->error_list[i].err_item_id;
				isis_LOG(lg, isis_FILE, isis_ERROR) << "p_errors->error_list[i].err_item_type: "<< p_errors->error_list[i].err_item_type;
			}

			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProFeatureWithoptionsCreate returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06063",err_str);  
		}
		return err;
	}


	ProError isis_ProFeatureWithoptionsRedefine(	ProAsmcomppath        *comp_path,
													ProFeature            *feature,
													ProElement             elemtree,
													ProFeatureCreateOptions *options,
													int                    flags,
													ProErrorlist          *p_errors)
										throw(isis::application_exception)
	{
		if (p_errors!=0)
			p_errors->error_number = 0;

		ProError err = ProFeatureWithoptionsRedefine ( comp_path, feature, elemtree, options, flags,  p_errors );

		if ( err != PRO_TK_NO_ERROR || p_errors->error_number != 0) 
		{
			
			isis_LOG(lg, isis_FILE, isis_ERROR) << "p_errors->error_number: " << p_errors->error_number;
			for (int i = 0; i < p_errors->error_number; ++i )
			{
				isis_LOG(lg, isis_FILE, isis_ERROR) << "p_errors->error_list[i].error: "<< p_errors->error_list[i].error;
				isis_LOG(lg, isis_FILE, isis_ERROR) << "p_errors->error_list[i].err_item_id: "<< p_errors->error_list[i].err_item_id;
				isis_LOG(lg, isis_FILE, isis_ERROR) << "p_errors->error_list[i].err_item_type: "<< p_errors->error_list[i].err_item_type;
			}

			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProFeatureWithoptionsRedefine returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06063",err_str);  
		}
		return err;
	}


	ProError isis_ProParameterInit(	ProModelitem *owner,
									const ProName       name, 
									ProParameter *param )
											throw(isis::application_exception)
	{

		ProError err = ProParameterInit ( owner, (wchar_t*) name, param );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char name_narrow[ISIS_CHAR_BUFFER_LENGTH];
			ProWstringToString(name_narrow,(wchar_t*)name); 
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProParameterInit returned ProError: %s(%d), Parameter Name: %s", ProToolKitError_string(err).c_str(), err, name_narrow );
			throw isis::application_exception("C06064",err_str);  
		}
		return err;
	}


	ProError isis_ProParameterValueSet(	ProParameter   *param, 
										ProParamvalue  *proval )
											throw(isis::application_exception)
	{

		ProError err = ProParameterValueSet ( param, proval );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProParameterValueSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06065",err_str);  
		}
		return err;
	}

	ProError isis_ProParameterCreate(	ProModelitem  *owner, 
										const ProName        name, 
										ProParamvalue *proval, 
										ProParameter  *param )
											throw(isis::application_exception)
	{
		ProError err = ProParameterCreate ( owner, (wchar_t *) name, proval, param );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProParameterCreate returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06066",err_str);  
		}
		return err;
	}

	ProError isis_ProParameterValueGet(	ProParameter   *param, 
                                      ProParamvalue  *proval )
											throw(isis::application_exception)
	{

		ProError err = ProParameterValueGet ( param, proval );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProParameterValueGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06067",err_str);  
		}
		return err;
	}


  	ProError isis_ProParameterScaledvalueSet(ProParameter   *param, 
                                            ProParamvalue  *proval,
                                            ProUnititem    *units)
										throw(isis::application_exception)
	{

		ProError err = ProParameterScaledvalueSet ( param, proval, units );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProParameterScaledvalueSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06068",err_str);  
		}
		return err;
	}

//ProSolidMassPropertyGet
//	ProUnitConversionGet
  	ProError isis_ProUnitInit(	ProMdl        mdl,
								const ProName       unit_name,
								ProUnititem*  unit)
										throw(isis::application_exception)
	{

		ProError err = ProUnitInit ( mdl, (wchar_t *)unit_name, unit );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProUnitInit returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06069",err_str);  
		}
		return err;
	}


	ProError isis_ProMacroLoad(	wchar_t* macro )
										throw(isis::application_exception)
	{

		ProError err = ProMacroLoad ( macro );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMacroLoad returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06070",err_str);  
		}
		return err;
	}



	ProError isis_ProSolidMassPropertyGet(ProSolid solid,
                                          const ProName  csys_name,
                                          ProMassProperty* mass_prop )
										throw(isis::application_exception)
	{

		ProError err = ProSolidMassPropertyGet ( solid, (wchar_t *)csys_name, mass_prop );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSolidMassPropertyGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06071",err_str);  
		}
		return err;
	}



	ProError isis_ProDirectoryChange(  ProPath path )
										throw(isis::application_exception)
	{
		
		ProError status ;
		switch ( status = ProDirectoryChange ( path ) ) {
		case PRO_TK_NO_ERROR: 
			isis_LOG(lg, isis_FILE, isis_INFO) << "The function successfully changed the directory.";
			break;
		case PRO_TK_BAD_INPUTS:
			{
				char  err_str[ERROR_STRING_BUFFER_LENGTH];
				sprintf( err_str, "exception : ProDirectoryChange : The specified path is invalid. ProError: %s: %s",
					ProToolKitError_string(status).c_str(), path);
				throw isis::application_exception("C06072",err_str); 
			}
		case PRO_TK_CANT_ACCESS:
			{
				char  err_str[ERROR_STRING_BUFFER_LENGTH];
				sprintf( err_str, "exception : ProDirectoryChange : You are unable to access the specified directory. ProError: %s: %s",
					ProToolKitError_string(status).c_str(), path);
				throw isis::application_exception("C06072",err_str); 
			}
		case PRO_TK_INVALID_NAME:
			{
				char  err_str[ERROR_STRING_BUFFER_LENGTH];
				sprintf( err_str, "exception : ProDirectoryChange : You specified an invalid name. ProError: %s: %s",
					ProToolKitError_string(status).c_str(), path);
				throw isis::application_exception("C06072",err_str); 
			}
		default:
			{
				char  err_str[ERROR_STRING_BUFFER_LENGTH];
				sprintf( err_str, "exception : ProDirectoryChange returned ProError: %s(%d) : %s",ProToolKitError_string(status).c_str(), status, path);
				throw isis::application_exception("C06072",err_str);  
			}
		}
		return status;
	}

	ProError isis_ProDirectoryCurrentGet( ProPath path )
										throw(isis::application_exception)
	{

		ProError err = ProDirectoryCurrentGet ( path );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProDirectoryCurrentGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06071",err_str);  
		}
		return err;
	}

	ProError isis_ProAsmcomppathTrfGet( ProAsmcomppath *p_path,
										ProBoolean   bottom_up,
										ProMatrix    transformation )
											throw(isis::application_exception)
	{

		ProError err = ProAsmcomppathTrfGet ( p_path, bottom_up, transformation );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAsmcomppathTrfGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06073",err_str);  

		}
		return err;
	}

	//// New 8/25/2010
	ProError isis_ProMdlPrincipalunitsystemGet( ProMdl          mdl,
												ProUnitsystem* principal_system)
											throw(isis::application_exception)
	{

		ProError err = ProMdlPrincipalunitsystemGet ( mdl, principal_system);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMdlPrincipalunitsystemGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06074",err_str);  

		}
		return err;
	}
	
	ProError isis_ProUnitsystemUnitGet( ProUnitsystem* system,
                                      ProUnitType     type,
                                      ProUnititem*   unit )
											throw(isis::application_exception)
	{

		ProError err = ProUnitsystemUnitGet ( system, type, unit );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProUnitsystemUnitGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06075",err_str);  
			
		}
		return err;
	}

	ProError isis_ProWstringCopy( wchar_t* source, wchar_t* target, int num_chars )
											throw(isis::application_exception)
	{

		ProError err = ProWstringCopy ( source, target, num_chars );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProWstringCopy returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06076",err_str);  
			
			
		}
		return err;
	}

	ProError isis_ProElementWstringSet( ProElement element, wchar_t* value)
													throw(isis::application_exception)
	{
		ProError err = ProElementWstringSet ( element, value );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProElementWstringSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06077",err_str);  
			
			
		}
		return err;
	}



	ProError isis_ProMaterialCreate(	ProSolid			part,
										const ProName				matl_name,
										ProMaterialdata		*p_matl_data,
										ProMaterial			*p_material )
											throw(isis::application_exception)
	{
		ProError err = ProMaterialCreate ( part, (wchar_t *)matl_name, p_matl_data, p_material  );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMaterialCreate returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06078",err_str);  					
		}
		return err;
	}


	ProError isis_ProMaterialDelete( ProMaterial      *p_material )
											throw(isis::application_exception)
	{
		ProError err = ProMaterialDelete ( p_material  );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMaterialDelete returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06079",err_str);  					
		}
		return err;
	}


	ProError isis_ProMaterialCurrentGet( ProSolid part,  ProMaterial  *p_material )
											throw(isis::application_exception)
	{
		ProError err = ProMaterialCurrentGet ( part, p_material );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMaterialCurrentGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06080",err_str);  
		}
		return err;
	}


	ProError isis_ProMaterialCurrentSet( ProMaterial  *p_material )
											throw(isis::application_exception)
	{
		ProError err = ProMaterialCurrentSet ( p_material );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMaterialCurrentSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06081",err_str);  	
		}
		return err;
	}


	ProError isis_ProPartMaterialsGet(	ProSolid         part,
										ProName      **p_matl_names_arr )
											throw(isis::application_exception)
	{
		ProError err = ProPartMaterialsGet ( part, p_matl_names_arr );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProPartMaterialsGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06082",err_str);  
		}
		return err;
	}

	ProError isis_ProMaterialPropertyGet(	ProMaterialItem *p_material,
											ProMaterialPropertyType prop_type,
											ProParamvalue    *p_value,
											ProUnititem      *p_units)
											throw(isis::application_exception)
	{
		ProError err = ProMaterialPropertyGet ( p_material, prop_type, p_value, p_units );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMaterialPropertyGet returned ProError: %s(%d), ProMaterialPropertyType %d: ", ProToolKitError_string(err).c_str(),err, prop_type );
			throw isis::application_exception("C06083",err_str);  
		}
		return err;
	}

	ProError isis_ProMaterialPropertySet(	ProMaterialItem *p_material,
											ProMaterialPropertyType prop_type,
											ProParamvalue    *p_value,
											ProUnititem      *p_units)
											throw(isis::application_exception)
	{
		ProError err = ProMaterialPropertySet ( p_material, prop_type, p_value, p_units );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMaterialPropertySet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06084",err_str);  
		}
		return err;
	}

	ProError isis_ProFemmeshExport( ProSolid			pro_solid,
                                    ProFemmeshData*		p_mesh_data,
                                    ProPath				file_name   )
											throw(isis::application_exception)
	{

		ProError err = ProFemmeshExport ( pro_solid, p_mesh_data, file_name );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProFemmeshExport returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06085",err_str);  
			
		}
		return err;
	}


	ProError isis_ProPointInit( ProSolid   owner_handle,
								int        point_id,
								ProPoint  *p_handle )
											throw(isis::application_exception)
	{

		ProError err = ProPointInit ( owner_handle, point_id,  p_handle);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProPointInit returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06086",err_str);  
		}
		return err;
	}

	ProError isis_ProPointCoordGet( ProPoint    point,
									ProVector   xyz_point   )
											throw(isis::application_exception)
	{

		ProError err = ProPointCoordGet ( point, xyz_point );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProPointCoordGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06087",err_str);  
		}
		return err;
	}

	ProError isis_ProPntTrfEval(	ProVector   in_point,
									ProMatrix   trf,
									ProVector   out_point  )
											throw(isis::application_exception)
	{

		ProError err = ProPntTrfEval ( in_point, trf, out_point );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProPntTrfEval returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06088",err_str);  
		}
		return err;
	}

	ProError isis_ProOutputFileWrite(	 ProMdl      model,
                                     const ProFileName name,
                                     ProImportExportFile   file_type,
                                     ProAppData  arg1,
                                     ProAppData  arg2,
                                     ProAppData  arg3,
                                     ProAppData  arg4 )
											throw(isis::application_exception)
	{

		ProError err = ProOutputFileWrite (  model,
											 (wchar_t *)name,
											 file_type,
											 arg1,
											 arg2,
											 arg3,
											 arg4 );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			isis::MultiFormatString  name_multi( name);
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProOutputFileWrite returned ProError: %s(%d), Output File Name: %s",ProToolKitError_string(err).c_str(), err, (const char *)name_multi );
			throw isis::application_exception("C06089",err_str);  
		}
		return err;
	}


	ProError isis_ProOutputFileMdlnameWrite(	 ProMdl      model,
                                     const ProFileName name,
                                     ProImportExportFile   file_type,
                                     ProAppData  arg1,
                                     ProAppData  arg2,
                                     ProAppData  arg3,
                                     ProAppData  arg4 )
											throw(isis::application_exception)
	{

		ProError err = ProOutputFileMdlnameWrite (  model,
											 (wchar_t *)name,
											 file_type,
											 arg1,
											 arg2,
											 arg3,
											 arg4 );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			isis::MultiFormatString  name_multi( name);
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProOutputFileMdlnameWrite returned ProError: %s(%d), Output File Name: %s",ProToolKitError_string(err).c_str(), err, (const char *)name_multi );
			throw isis::application_exception("C06089",err_str);  
		}
		return err;
	}


	ProError isis_ProOutputAssemblyConfigurationIsSupported( ProIntf3DExportType file_type,
           											ProOutputAssemblyConfiguration configuration,
													ProBoolean*  is_supported)
													throw(isis::application_exception)
	{

		ProError err =  ProOutputAssemblyConfigurationIsSupported(	file_type,
           															configuration,
																	is_supported);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProOutputAssemblyConfigurationIsSupported returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06089",err_str);  
		}
		return err;

	}


	ProError   isis_ProOutputBrepRepresentationAlloc( ProOutputBrepRepresentation* representation)
														throw(isis::application_exception)
	{

		ProError err =  ProOutputBrepRepresentationAlloc( representation);


		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProOutputBrepRepresentationAlloc returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06089",err_str);  
		}
		return err;
	}






	ProError   isis_ProOutputBrepRepresentationFlagsSet(
								ProOutputBrepRepresentation representation,
								ProBoolean as_wireframe,
								ProBoolean as_surfaces,
								ProBoolean as_solid,
								ProBoolean as_quilts)
														throw(isis::application_exception)
	{

		ProError err =  ProOutputBrepRepresentationFlagsSet( representation, as_wireframe,
															 as_surfaces, as_solid, as_quilts);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProOutputBrepRepresentationFlagsSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06089",err_str);  
		}
		return err;
	}

	ProError   isis_ProOutputBrepRepresentationFree( ProOutputBrepRepresentation representation)
																	throw(isis::application_exception)
	{

		ProError err = ProOutputBrepRepresentationFree( representation);


		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProOutputBrepRepresentationFree returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06089",err_str);  
		}
		return err;
	}




	ProError   isis_ProOutputBrepRepresentationIsSupported( ProIntf3DExportType file_type,
          													ProOutputBrepRepresentation representation,
															ProBoolean*  is_supported)
																	throw(isis::application_exception)
	{

		ProError err = ProOutputBrepRepresentationIsSupported( file_type, representation, is_supported);


		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProOutputBrepRepresentationIsSupported returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06089",err_str);  
		}
		return err;
	}



	ProError   isis_ProOutputInclusionWithOptionsSet(	ProOutputInclusion inclusion,
													double             *parameters,
													int                config_flags)
														throw(isis::application_exception)
	{

		ProError err =  ProOutputInclusionWithOptionsSet( inclusion, parameters, config_flags);


		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProOutputInclusionWithOptionsSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06089",err_str);  
		}
		return err;
	}


	ProError   isis_ProOutputInclusionAlloc( ProOutputInclusion* inclusion)
														throw(isis::application_exception)
	{

		ProError err =  ProOutputInclusionAlloc( inclusion);


		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : isis_ProOutputInclusionAlloc returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06089",err_str);  
		}
		return err;
	}

	ProError   isis_ProOutputInclusionFlagsSet( ProOutputInclusion inclusion,
														 ProBoolean include_datums,
														 ProBoolean include_blanked,
														 ProBoolean include_facetted)
														throw(isis::application_exception)
	{

		ProError err = ProOutputInclusionFlagsSet( inclusion, include_datums, include_blanked, include_facetted);


		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProOutputInclusionFlagsSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06089",err_str);  
		}
		return err;
	}


	ProError   isis_ProOutputInclusionFree (ProOutputInclusion inclusion)
														throw(isis::application_exception)
	{

		ProError err =  ProOutputInclusionFree (inclusion);


		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProOutputInclusionFree returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06089",err_str);  
		}
		return err;
	}

	ProError isis_ProIntf3DFileWrite(	ProSolid solid,
        								ProIntf3DExportType file_type,
										ProPath output_file,
        								ProOutputAssemblyConfiguration configuration,
										ProSelection  reference_csys, 
        								ProOutputBrepRepresentation brep_representation, 
										ProOutputInclusion inclusion, 
        								ProOutputLayerOptions layer_options )
											throw(isis::application_exception)
	{
	
		ProError err = ProIntf3DFileWrite (  solid,
											 file_type,
											 output_file,
											 configuration,
											 reference_csys,
											 brep_representation,
											 inclusion,
											 layer_options);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			isis::MultiFormatString  name_multi( output_file);
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProIntf3DFileWrite returned ProError: %s(%d), Output File Name: %s",ProToolKitError_string(err).c_str(), err, (const char *)name_multi );
			throw isis::application_exception("C06089",err_str);  
		}
		return err;
	}


	ProError isis_ProConfigoptSet(	const ProName option,
                                    ProPath option_value  )
											throw(isis::application_exception)
	{

		ProError err = ProConfigoptSet ( (wchar_t *)option, option_value );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProConfigoptSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06090",err_str);  
		}
		return err;
	}



	ProError isis_ProRasterFileWrite( int            window_id,
                                      ProRasterDepth depth,
                                      double         width,
                                      double         height,
                                      ProDotsPerInch dots_per_inch,
                                      ProRasterType  type,
                                      ProPath        output_file )
											throw(isis::application_exception)
	{

		ProError err = ProRasterFileWrite ( window_id,
                                            depth,
                                            width,
                                            height,
                                            dots_per_inch,
                                            type,
                                            output_file );


		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProRasterFileWrite returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06091",err_str);  
		}
		return err;
	}

	ProError isis_ProWindowCurrentGet( int* p_window_id )
											throw(isis::application_exception)
	{

		ProError err = ProWindowCurrentGet ( p_window_id );


		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProWindowCurrentGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06092",err_str);  
		}
		return err;
	}

	ProError isis_ProSolidOutlineGet( ProSolid  p_solid,
										Pro3dPnt  r_outline_points[2])
											throw(isis::application_exception)
	{
		ProError err = ProSolidOutlineGet ( p_solid,
											r_outline_points );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSolidOutlineGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06093",err_str);  
		}
		return err;
	}

	ProError isis_ProSolidOutlineCompute(	ProSolid              p_solid, 
                                            ProMatrix             matrix,
                                            ProSolidOutlExclTypes excludes[],
                                            int                   num_excludes,
                                            Pro3dPnt r_outline_points[2])
											throw(isis::application_exception)
	{
		ProError err = ProSolidOutlineCompute ( p_solid, matrix, excludes, num_excludes, r_outline_points );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSolidOutlineCompute returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06093",err_str);  
		}
		return err;
	}


	ProError isis_ProAsmcompPositionSet(	ProAsmcomppath* component_path,
											ProAsmcomp*     component,
											ProMatrix      position )
											throw(isis::application_exception)
	{
		ProError err = ProAsmcompPositionSet (	component_path,
												component,
												position);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAsmcompPositionSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06094",err_str);  
		}
		return err;
	}

	ProError isis_ProFeatureDelete(	ProSolid    solid,
									int        *feat_ids,
									int         feat_count,
									ProFeatureDeleteOptions *delete_opts,
									int         num_opts)
											throw(isis::application_exception)
	{
		ProError err = ProFeatureDelete (	solid,
											feat_ids,
											feat_count,
											delete_opts,
											num_opts);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProFeatureDelete returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06095",err_str);  
		}
		return err;
	}


	ProError isis_ProFeatureWithoptionsDelete(	
									ProSolid                 solid,
                                    int                     *feat_list,
                                    ProFeatureDeleteOptions *opts,
                                    int                      flags)
											throw(isis::application_exception)
	{
		ProError err = ProFeatureWithoptionsDelete (	solid,
														feat_list,
														opts,
														flags );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProFeatureWithoptionsDelete returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06096",err_str);  
		}
		return err;
	}

	ProError isis_ProMdlNameGet(ProMdl     handle, 
                                ProName    name)
											throw(isis::application_exception)
	{
		ProError err = ProMdlNameGet (	handle, name );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMdlNameGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06097",err_str);  
		}
		return err;
	}

	ProError isis_ProMdlTypeGet(ProMdl model, 
                                ProMdlType* p_type)
											throw(isis::application_exception)
	{
		ProError err = ProMdlTypeGet (	model, p_type );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMdlTypeGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06097",err_str);  
		}
		return err;
	}

	ProError isis_ProMdlCopy(ProMdl      handle,
                             ProName     new_name,
                             ProMdl     *p_new_handle)
											throw(isis::application_exception)
	{
		ProError err = ProMdlCopy (	handle, new_name, p_new_handle );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProMdlCopy returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06097",err_str);  
		}
		return err;
	}


	ProError isis_ProElementFree(ProElement     *p_elem )
											throw(isis::application_exception)
	{
		ProError err = ProElementFree (	p_elem );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProElementFree returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06098",err_str);  
		}
		return err;
	}


	ProError isis_ProDtmcsysTransformfileRead( wchar_t* file_name, ProElement elem_tree )
											               throw(isis::application_exception)
	{
		ProError err = ProDtmcsysTransformfileRead ( file_name, elem_tree );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProDtmcsysTransformfileRead returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06099",err_str);  
		}
		return err;
	}

	ProError isis_ProSelect( char              option[],
			   int               max_count,
			   ProSelection     *p_in_sel,
			   ProSelFunctions  *sel_func,
			   ProSelectionEnv   sel_env,
			   ProSelAppAction   appl_act_data,
			   ProSelection    **p_sel_array,
			   int              *p_n_sels  )
											   throw(isis::application_exception)
	{
		ProError err = ProSelect (  option,
									max_count,
									p_in_sel,
									sel_func,
									sel_env,
									appl_act_data,
									p_sel_array,
									p_n_sels  );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSelect returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06100",err_str);  
		}
		return err;
	}

	ProError isis_ProCsysIdGet( ProCsys csys, int *p_id )
											               throw(isis::application_exception)
	{
		ProError err = ProCsysIdGet ( csys, p_id );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProCsysIdGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06101",err_str);  
		}
		return err;
	}

	ProError isis_ProAxisIdGet( ProAxis  axis, int *p_id ) throw(isis::application_exception)
	{
		ProError err = ProAxisIdGet ( axis, p_id );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAxisIdGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}



	ProError isis_ProIntfimportModelWithOptionsCreate( ProPath  import_file, 
                                                      ProPath  profile, 
                                                      ProIntfImportType type, 
                                                      ProMdlType create_type, 
                                                      ProImportRepType rep_type, 
                                                      const ProName  new_model_name, 
                                                      ProIntfimportLayerFilter filter_func, 
                                                      ProAppData application_data, 
                                                      ProMdl* created_model ) throw(isis::application_exception)
	{
		ProError err = ProIntfimportModelWithOptionsCreate ( import_file,
															 profile,
															 type,
															 create_type,
															 rep_type,
															 (wchar_t *)new_model_name,
															 filter_func,
															 application_data,
															 created_model);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProIntfimportModelWithOptionsCreate returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06103",err_str);  
		}
		return err;
	}

	ProError isis_ProFeatureStatusGet(	ProFeature    *p_feat_handle,
										ProFeatStatus *p_status) throw(isis::application_exception)
	{
		ProError err = ProFeatureStatusGet ( p_feat_handle, p_status );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProFeatureStatusGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}

	// Works only on assemblies
	ProError isis_ProSimprepActivate( ProSolid    p_solid, 
                                    ProSimprep *p_simp_rep) throw(isis::application_exception)
	{
		ProError err = ProSimprepActivate ( p_solid, p_simp_rep );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSimprepActivate returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}


	// Works only on assemblies
	ProError isis_ProAssemblySimprepRetrieve(	ProFamilyName    assem_name, 
												ProName          simp_rep_name,
												ProSimprepdata  *simp_rep_data,
												ProAssembly     *p_assem) throw(isis::application_exception)
	{
		ProError err = ProAssemblySimprepRetrieve( assem_name, simp_rep_name, simp_rep_data, p_assem );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProAssemblySimprepRetrievereturned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}

	// Works only on parts
	ProError isis_ProPartSimprepRetrieve(	ProFamilyName   part_name, 
											ProSimprepType  type,
											ProName         simp_rep_name,
											ProPart        *p_part) throw(isis::application_exception)
	{
		ProError err = ProPartSimprepRetrieve ( part_name, type, simp_rep_name, p_part );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProPartSimprepRetrieve returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}


	ProError isis_ProSimprepdataGet(	ProSimprep			*p_simp_rep,
										ProSimprepdata		**p_data ) throw(isis::application_exception)
	{
		ProError err = ProSimprepdataGet ( p_simp_rep, p_data );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSimprepdataGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}


	ProError isis_ProSimprepdataitemsVisit(	ProSimprepdata *p_data, 
                                           ProFunction     filter,
                                           ProFunction     action,
                                           ProAppData      app_data ) throw(isis::application_exception)
	{
		ProError err = ProSimprepdataitemsVisit ( p_data, filter, action, app_data );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSimprepdataitemsVisit returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}

	ProError isis_ProSimprepdataFree(	ProSimprepdata   **p_data ) throw(isis::application_exception)
	{
		ProError err = ProSimprepdataFree ( p_data );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSimprepdataFree returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}

	ProError isis_ProSolidRetrievalErrorsGet(	ProSolid solid,
					     ProSolidretrievalerrs* errs) throw(isis::application_exception)
	{
		ProError err = ProSolidRetrievalErrorsGet( solid, errs );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSolidRetrievalErrorsGetreturned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}

	


	ProError isis_ProFitGlobalinterferenceCompute(	ProAssembly assem,
													ProFitComputeSetup comp_setup,
													ProBoolean set_facets,
													ProBoolean set_quilts,
													ProBoolean fast_calculation, 
													ProInterferenceInfo **interf_info) throw(isis::application_exception)
	{
		ProError err =  ProFitGlobalinterferenceCompute ( assem, comp_setup, set_facets, set_quilts, fast_calculation, interf_info );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProFitGlobalinterferenceCompute returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}


	ProError isis_ProInterferenceInfoProarrayFree( ProInterferenceInfo *interf_info ) throw(isis::application_exception)
	{
		ProError err =  ProInterferenceInfoProarrayFree ( interf_info );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProInterferenceInfoProarrayFree returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}


	ProError isis_ProFitInterferencevolumeCompute(    ProInterferenceData interf_data,
													  double *volume ) throw(isis::application_exception)
	{
		ProError err =  ProFitInterferencevolumeCompute ( interf_data, volume );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProFitInterferencevolumeCompute returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}


	ProError isis_ProSelectionAsmcomppathGet(	ProSelection	selection,
												ProAsmcomppath	*p_cmp_path ) throw(isis::application_exception)
	{
		ProError err =  ProSelectionAsmcomppathGet ( selection, p_cmp_path );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSelectionAsmcomppathGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}

	ProError isis_ProSelectionModelitemGet(	ProSelection selection,
											ProModelitem *p_mdl_item) throw(isis::application_exception)
	{
		ProError err =  ProSelectionModelitemGet ( selection, p_mdl_item );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSelectionModelitemGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}


	ProError isis_ProElemtreeWrite(		ProElement         elemtree, 
										ProElemtreeFormat  format,
										ProPath            output_file ) throw(isis::application_exception)
	{
		ProError err =  ProElemtreeWrite ( elemtree, format, output_file );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			MultiFormatString FilePathandName(output_file);
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProElemtreeWrite returned ProError: %s(%d), File Name: %s",ProToolKitError_string(err).c_str(), err, (const char*) FilePathandName );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}

	 //Creo Parametric 2.0 it is 31.
	ProError isis_ProEngineerReleaseNumericversionGet(	int* ver ) throw(isis::application_exception)
	{
		ProError err =  ProEngineerReleaseNumericversionGet ( ver );

		if ( err != PRO_TK_NO_ERROR ) 
		{
				char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProEngineerReleaseNumericversionGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}

	ProError isis_ProSurfaceInit (	ProMdl        p_owner_handle,
									int           surf_id,
									ProSurface   *p_surface)
																throw(isis::application_exception)
	{
		ProError err =  ProSurfaceInit ( p_owner_handle, surf_id, p_surface);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSurfaceInit returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}

	ProError isis_ProPoint3dOnsurfaceFind(	ProPoint3d      xyz,        
											ProSurface      surface,   
											ProBoolean     *on_surf,  
											ProPoint3d      closest_pt)
																throw(isis::application_exception)
	{
		ProError err =  ProPoint3dOnsurfaceFind(xyz, surface,  on_surf, closest_pt);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProPoint3dOnsurfaceFind returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}


	ProError isis_ProModelitemMdlGet (		ProModelitem    *p_model_item, 
											ProMdl          *p_owner) 
																throw(isis::application_exception)	
	{
		ProError err =  ProModelitemMdlGet(p_model_item, p_owner);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProModelitemMdlGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}

	// Note - The following function is from UtilCollect.c
	ProError isis_ProUtilCollectSolidSurfaces ( ProSolid	    p_solid,		//	In:  The handle to the solid */
												ProSurface	    **p_surfaces	//	Out: ProArray with collected surfaces. 
																				//	The function allocates memory 
																				//	for this argument, but you must 
																				//	free it. To free the memory, 
																				//	call the function ProArrayFree()*/
																				)
																		throw (isis::application_exception)
	{
		ProError err =  ProUtilCollectSolidSurfaces ( p_solid, p_surfaces);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : isis_ProUtilCollectSolidSurfaces returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;

	}

	ProError  isis_ProSectionNameGet (ProSection section, ProName r_name) throw (isis::application_exception)
	{
		ProError err =  ProSectionNameGet (section, r_name);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSectionNameGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;

	}

	ProError isis_ProSectionNameSet (ProSection section, wchar_t new_name[]) throw (isis::application_exception)
	{
		ProError err =  ProSectionNameSet (section, new_name);

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSectionNameSet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}



	ProError isis_ProSectionEntityFromProjection (	ProSection		section,
													ProSelection	ref_geometry,
													int				*r_ent_id) throw (isis::application_exception)
	{

		ProError err =  ProSectionEntityFromProjection (section, ref_geometry, r_ent_id);
		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSectionEntityFromProjection returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}


	ProError isis_ProSectionEntityGet (	ProSection section, 
										int ent_id,
										Pro2dEntdef **POutEnt) throw (isis::application_exception)
	{

		ProError err =  ProSectionEntityGet ( section, ent_id, POutEnt );
		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSectionEntityGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}


	ProError isis_ProSectionLocationGet (	ProSection section,
											ProMatrix  location_matrix) throw (isis::application_exception)
	{

		ProError err =  ProSectionLocationGet ( section, location_matrix );
		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSectionLocationGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}


	ProError isis_ProSectionEntityAdd ( ProSection section,
										Pro2dEntdef *entity2d,
										int *r_ent_id) throw (isis::application_exception)
	{

		ProError err =  ProSectionEntityAdd ( section, entity2d, r_ent_id );
		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSectionEntityAdd returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception("C06102",err_str);  
		}
		return err;
	}

	std::string RetrieveSectionErrors( ProWSecerror errors)
	{
		ProError err;
		std::string detailErrorMsgs;
		
		int n_errors;
		err = ProSecerrorCount (&errors, &n_errors);

		//std::cout << std::endl << "--------------------------> Error Count: " << n_errors;

		if ( err == PRO_TK_NO_ERROR )
		{
			for (int e = 0; e < n_errors; e++)
			{
				ProMsg              wmsg;
				char                msg[PRO_PATH_SIZE];
				err = ProSecerrorMsgGet (errors, e, wmsg);
				if ( err == PRO_TK_NO_ERROR )
				{
					ProWstringToString (msg, wmsg);
					detailErrorMsgs += " " + std::string(msg);
				}
			}
		}
		return detailErrorMsgs;
	}

	ProError isis_ProSectionAutodim ( ProSection section ) throw (isis::application_exception)
	{
		ProWSecerror        sec_errors;
		ProSecerrorAlloc	(&sec_errors);

		ProError err =  ProSectionAutodim ( section, &sec_errors );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			std::string detailErrorMsgs = RetrieveSectionErrors(sec_errors);

			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSectionAutodim returned ProError: %s(%d,), Optional Error Descriptions: %s",ProToolKitError_string(err).c_str(), err, detailErrorMsgs );
			throw isis::application_exception("C06102",err_str);  
		}

		ProSecerrorFree(&sec_errors);
		return err;
	}


	ProError isis_ProSectionSolve ( ProSection section) throw (isis::application_exception)
	{
		ProWSecerror        sec_errors;
		ProSecerrorAlloc(&sec_errors);

		ProError err =  ProSectionSolve ( section, &sec_errors );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			std::string detailErrorMsgs = RetrieveSectionErrors(sec_errors);

			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSectionSolve returned ProError: %s(%d,), Optional Error Descriptions: %s",ProToolKitError_string(err).c_str(), err, detailErrorMsgs );
			throw isis::application_exception("C06102",err_str);  
		}

		ProSecerrorFree(&sec_errors);
		return err;
	}


	ProError isis_ProSectionRegenerate ( ProSection section) throw (isis::application_exception)
	{
		ProWSecerror        sec_errors;
		ProSecerrorAlloc(&sec_errors);

		ProError err =  ProSectionRegenerate ( section, &sec_errors );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			std::string detailErrorMsgs = RetrieveSectionErrors(sec_errors);

			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "exception : ProSectionRegenerate returned ProError: %s(%d,), Optional Error Descriptions: %s",ProToolKitError_string(err).c_str(), err, detailErrorMsgs );
			throw isis::application_exception("C06102",err_str);  
		}
		ProSecerrorFree(&sec_errors);
		return err;
	}

	/*
	
	//ProMech* functions require a special Mechanica license
	
	ProError isis_ProMechanicaEnter(	ProSolid         solid,
										ProMechanicaMode mode,
										ProBool          fem )
										throw(isis::application_exception)
	{

		ProError err = ProMechanicaEnter ( solid, mode, fem );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "ProMechanicaEnter returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception(err_str);  
		}
		return err;
	}

	
	ProError isis_ProMechgeomrefPathGet(	ProMechGeomref entity,
											ProAsmcomppath* path)
										throw(isis::application_exception)
	{

		ProError err = ProMechgeomrefPathGet ( entity, path );

		if ( err != PRO_TK_NO_ERROR ) 
		{
			char  err_str[ERROR_STRING_BUFFER_LENGTH];
			sprintf( err_str, "ProMechgeomrefPathGet returned ProError: %s(%d)",ProToolKitError_string(err).c_str(), err );
			throw isis::application_exception(err_str);  
		}
		return err;
	}
	
*/
}
