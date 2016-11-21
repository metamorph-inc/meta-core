/*! \file isis_ptc_toolkit_functions.h
    \brief Wrapper functions for Creo Toolkit functions.

	The Creo Toolkit functions are C functions; and therefore, do no support exceptions.  
	The wrapper functions throw an exception if the call to the Toolkit function returns 
	a non-zero error code.  See the Creo Toolkit User's Guide (tkuse.pdf, typically located at 
	"C:\Program Files\PTC\Creo 1.0\Common Files\M030\protoolkit" ) for a description of each 
	the Toolkit functions.
*/

#ifndef ISIS_PTC_TOOLKIT_FUNCTIONS_H
#define ISIS_PTC_TOOLKIT_FUNCTIONS_H

#pragma warning( disable : 4290 )  // a future feature : exception specification, i.e. throw

#include <isis_application_exception.h>
#include <isis_include_ptc_headers.h>

namespace isis
{
	

    extern ProError isis_ProEngineerStart(	char* proe_path,
											char* prodev_text_path ) 
											throw (isis::application_exception);

	extern ProError  isis_ProMdlRetrieve( const ProFamilyName name, 
										  ProMdlType    type,
										  ProMdl       *p_handle)
										  throw (isis::application_exception); 

	extern	ProError isis_ProMdlDisplay( ProMdl    handle )
								  throw (isis::application_exception);

	extern ProError isis_ProWindowRefresh( int window_id )
								  throw (isis::application_exception);

	extern ProError isis_ProWindowRepaint ( int window_id )
								  throw (isis::application_exception);

	extern  ProError isis_ProMdlWindowGet( ProMdl mdl, int* window_id )
								  throw (isis::application_exception);

	extern ProError isis_ProWindowCurrentSet(int window_id )
								  throw (isis::application_exception);

	extern ProError isis_ProWindowActivate(int window_id )
								  throw (isis::application_exception);

	extern	ProError isis_ProMdlDelete( ProMdl    handle )
								  throw (isis::application_exception);

	extern	ProError isis_ProMdlErase  (ProMdl    handle) throw(isis::application_exception);

	extern ProError isis_ProMdlEraseNotDisplayed()
								  throw (isis::application_exception);

	extern ProError isis_ProMdlfileCopy ( ProMdlType   mdl_type,
								   const ProName      from_name,
								   const ProName      to_name );

	extern ProError isis_ProSolidCreate  (	const ProName     name, 
											ProType     type,
											ProSolid   *p_handle)
											throw (isis::application_exception);

	extern ProError isis_ProEngineerEnd( void ) throw (isis::application_exception); 

	// extern FILE *isis_PTApplsUnicodeFopen(const char *filename, const char *mode) throw(isis::application_exception); 

	extern ProError isis_ProMdlDataGet( ProMdl handle, ProMdldata *p_data) throw(isis::application_exception);

	extern ProError isis_ProAsmcomppathMdlGet( ProAsmcomppath *p_path, ProMdl *p_model) throw(isis::application_exception);

	extern ProError isis_ProSolidFeatVisit ( ProSolid     p_handle, 
											 ProFeatureVisitAction  visit_action,
											 ProFeatureFilterAction  filter_action,
											 ProAppData   app_data) throw(isis::application_exception);

	extern ProError isis_ProAsmcompMdlGet ( ProAsmcomp *p_feat_handle,
											ProMdl     *p_mdl_handle) throw(isis::application_exception);

	extern ProError isis_ProFeatureTypeGet  (ProFeature  *p_feat_handle, ProFeattype *p_type) throw(isis::application_exception);

	extern ProError isis_ProAsmcompAssemble ( ProAssembly   owner_assembly,
				     ProSolid           component_model,
				     ProMatrix           init_pos,
				     ProAsmcomp *feature ) throw(isis::application_exception);

	extern ProError isis_ProAsmcompCreateCopy ( ProAssembly  assembly,
				      ProName      comp_name,
				      ProMdlType   comp_type,
				      ProMdl       template_model,
				      ProBoolean   leave_unplaced,
				      ProAsmcomp  *p_comp_handle) throw(isis::application_exception);

	extern ProError isis_ProAsmcompFillFromMdl ( ProAsmcomp *comp_handle,
										  ProMdl      template_model) throw(isis::application_exception);


	extern ProError isis_ProMdlSave  (ProMdl    handle) throw(isis::application_exception);

	extern ProError isis_ProModelitemByNameInit ( ProMdl         mdl, 
                                         ProType        type, 
                                         const ProName        name, 
                                         ProModelitem*  p_item ) throw(isis::application_exception); 

	extern ProError isis_ProModelitemNameGet (	ProModelitem *p_item,
												ProName       name ) throw(isis::application_exception);


	extern ProError isis_ProFeatureInit ( ProSolid     p_owner_handle, 
                                  int          feat_id, 
                                  ProFeature  *p_feat_handle ) throw(isis::application_exception);


	extern ProError isis_ProElempathAlloc ( ProElempath *p_elem_path  ) throw(isis::application_exception);

	extern ProError isis_ProElempathFree ( ProElempath *p_elem_path  ) throw(isis::application_exception);

	extern ProError isis_ProElempathDataSet (	ProElempath       elem_path,
												ProElempathItem  *elempath_data,
												int               path_size  ) 
															throw(isis::application_exception);

	extern ProError isis_ProValueDataGet ( ProValue       value,
									ProValueData  *value_data ) throw(isis::application_exception);

	extern ProError isis_ProElementArrayCount (ProElement    elemtree,
                                      ProElempath   elempath,
                                      int          *p_array_count) throw(isis::application_exception);

	extern ProError isis_ProFeatureElemtreeExtract (	ProFeature *feature,
												ProAsmcomppath *path,
												ProFeatureElemtreeExtractOptions opts,
												ProElement *p_elem) throw(isis::application_exception);

	extern ProError isis_ProFeatureElemtreeFree (	ProFeature *feature,
													ProElement  p_elem) throw(isis::application_exception);

	extern ProError isis_ProFeatureRedefine (	ProAsmcomppath   *comp_path,
										ProFeature       *feature,
										ProElement        elemtree,
										ProFeatureCreateOptions options[],
										int               num_opts,
										ProErrorlist     *p_errors ) throw(isis::application_exception);


	extern ProError isis_ProGeomitemFeatureGet (	ProGeomitem  *p_geom_item, 
													ProFeature   *p_feat_handle  ) 
															throw(isis::application_exception);

	extern ProError isis_ProFeatureElemValueGet (	ProFeature    *feature,
													ProElempath    elempath,
													ProValue      *p_elemvalue ) throw(isis::application_exception);

	extern ProError isis_ProMdlCurrentGet ( ProMdl     *p_handle) throw(isis::application_exception);

	extern ProError isis_ProModelitemInit ( ProMdl         p_owner_handle, 
									 int            item_id, 
									 ProType        item_type, 
									 ProModelitem  *p_handle) throw(isis::application_exception);

	extern ProError isis_ProMdlToModelitem (	ProMdl mdl, 
										ProModelitem* p_model_item ) throw(isis::application_exception);

	extern ProError isis_ProArrayAlloc ( int n_objs,
                                  int obj_size,
                                  int reallocation_size,
                                  ProArray* p_array) throw(isis::application_exception);


	extern	ProError isis_ProArrayFree( ProArray* p_array) throw(isis::application_exception);

	extern ProError isis_ProArraySizeGet( ProArray in_array,
									int*     p_size) throw(isis::application_exception);

	extern ProError isis_ProAsmcomppathInit ( ProSolid       p_solid_handle,
											  ProIdTable     memb_id_tab,
											  int            table_size,
											  ProAsmcomppath *p_handle) throw(isis::application_exception);

	extern ProError isis_ProSelectionAlloc (	ProAsmcomppath *p_cmp_path,
										ProModelitem *p_mdl_itm,
										ProSelection *p_selection) throw(isis::application_exception);

	extern ProError isis_ProSelectionCopy( ProSelection  from_selection, ProSelection  *p_to_selection ) 
																	throw(isis::application_exception);

	extern ProError isis_ProFeatureCreate (	ProSelection     model,
										ProElement       elemtree,
										ProFeatureCreateOptions options[],
										int              num_opts,
										ProFeature      *p_feature,
										ProErrorlist    *p_errors ) throw(isis::application_exception);

	extern	ProError isis_ProSelectionFree ( ProSelection *p_selection) 
											throw(isis::application_exception);

	extern ProError isis_ProAsmcompconstraintAlloc (	ProAsmcompconstraint *p_constraint) throw(isis::application_exception);

	extern ProError isis_ProAsmcompconstraintFree (ProAsmcompconstraint constraint) throw(isis::application_exception);

	extern ProError isis_ProAsmcompConstrRemove (	ProAsmcomp *p_feat_handle, int  index ) throw(isis::application_exception);

	extern ProError isis_ProAsmcompconstraintTypeSet (	ProAsmcompconstraint constraint,
												ProAsmcompConstrType type) throw(isis::application_exception);

	extern ProError isis_ProAsmcompconstraintAsmreferenceSet (	ProAsmcompconstraint constraint,
													ProSelection asm_ref,
													ProDatumside asm_orient) throw(isis::application_exception);

	extern ProError isis_ProAsmcompconstraintAttributesSet (	 ProAsmcompconstraint constraint ,
														int attributes) throw(isis::application_exception);

	extern ProError isis_ProAsmcompconstraintCompreferenceSet (	ProAsmcompconstraint constraint,
													ProSelection comp_ref,
													ProDatumside comp_orient) throw(isis::application_exception);

	extern ProError isis_ProAsmcompRegenerate (	ProAsmcomp *p_comp_handle,
												ProBoolean  update_soft) 
														throw(isis::application_exception);

	extern ProError isis_ProAsmcompPositionGet (	ProAsmcomp*     component,
											ProMatrix    position) 
														throw(isis::application_exception);

	extern ProError isis_ProArrayObjectAdd (	ProArray* p_array,
												int       index,
												int       n_objects,
												void*     p_object) throw(isis::application_exception);

	extern	ProError isis_ProAsmcompConstraintsSet(	ProAsmcomppath* component_path, 
											ProAsmcomp *component,
											ProAsmcompconstraint *p_constraints ) throw(isis::application_exception);

	extern	ProError isis_ProSolidRegenerate(	ProSolid    p_handle,
                                                  int  flags ) throw(isis::application_exception);

	extern	ProError isis_ProSolidRegenerationstatusGet( ProSolid                    solid,
												 ProSolidRegenerationStatus* regen_status ) 
																throw(isis::application_exception);


	extern	ProError isis_ProAsmcompconstraintOffsetSet(	ProAsmcompconstraint constraint,
													double offset) throw(isis::application_exception);

	extern	ProError isis_ProUnitsystemTypeGet( ProUnitsystem*     system,
												ProUnitsystemType* type)
												throw(isis::application_exception);

	extern  ProError isis_ProMdlPrincipalunitsystemSet( ProMdl             mdl,
												ProUnitsystem*     new_system,
												ProUnitConvertType type,
												ProBoolean         ignore_param_units,
												int                regeneration_flags)
												throw(isis::application_exception);

	extern	ProError isis_ProElementAlloc (		ProElemId name_id,
												ProElement *p_elem)
												throw(isis::application_exception);

	extern	ProError isis_ProElementValueSet (	ProElement  elem,
												ProValue    value)
												throw(isis::application_exception);

	extern ProError isis_ProElementValueGet (	ProElement       elem,
												ProValue        *p_value)
												throw(isis::application_exception);

	extern ProError isis_ProFeatureWithoptionsCreate(	ProSelection  model,
                                             ProElement               elemtree,
                                             ProFeatureCreateOptions *options,
                                             int                      flags,
                                             ProFeature              *p_feature,
                                             ProErrorlist            *p_errors)
											 throw(isis::application_exception);

	extern ProError isis_ProFeatureWithoptionsRedefine(	ProAsmcomppath        *comp_path,
													ProFeature            *feature,
													ProElement             elemtree,
													ProFeatureCreateOptions *options,
													int                    flags,
													ProErrorlist          *p_errors)
										throw(isis::application_exception);

	extern ProError isis_ProElementReferenceGet (	ProElement                 element,
                                         ProElementReferenceOptions options,
                                         ProReference*              reference)
												throw(isis::application_exception);

	extern	ProError isis_ProElementIntegerSet(	ProElement element, int value)
												throw(isis::application_exception);

	extern	ProError isis_ProElementReferencesSet(	ProElement    element,
													ProReference* references)
														throw(isis::application_exception);

	extern	ProError isis_ProElemtreeElementAdd( ProElement     elemtree,
												 ProElempath    elempath,
												 ProElement     elem )
												  throw(isis::application_exception);

	extern ProError isis_ProElemtreeElementGet(	ProElement   elemtree,
												ProElempath  elempath,
												ProElement  *p_elem)
												throw(isis::application_exception);

	extern ProError isis_ProElementIntegerGet(	ProElement               element, 
												ProElementIntegerOptions options, 
												int*                     value)
													throw(isis::application_exception);

	extern ProError isis_ProElementDoubleGet(	ProElement              element, 
												ProElementDoubleOptions options, 
											double*                 value)
												throw(isis::application_exception);

	extern void isis_AddElementtoElemTree(ProElemId id, ProElement *parent, ProValueData *elem_data, ProElement *elem )
																								throw(isis::application_exception);

	
	extern	ProError isis_ProValueAlloc( ProValue *p_value )
										throw(isis::application_exception);

	extern ProError isis_ProValueFree( ProValue value )
										throw(isis::application_exception);

	extern	ProError isis_ProValueDataSet(	ProValue       value,
											ProValueData  *value_data )
												throw(isis::application_exception);

	extern ProError isis_ProElementArrayGet(	ProElement      elemtree,
												ProElempath     elempath,
												ProElement    **p_array_elems )
													throw(isis::application_exception);

	extern	ProError isis_ProMacroLoad(	wchar_t* macro )
										throw(isis::application_exception);

	extern		ProError isis_ProParameterInit(	ProModelitem *owner,
												const ProName       name, 
												ProParameter *param )
													throw(isis::application_exception);

	extern	ProError isis_ProParameterValueSet(	ProParameter   *param, 
										ProParamvalue  *proval )
											throw(isis::application_exception);

	extern ProError isis_ProParameterCreate(	ProModelitem  *owner, 
										const ProName        name, 
										ProParamvalue *proval, 
										ProParameter  *param )
											throw(isis::application_exception);

	extern	ProError isis_ProParameterValueGet(	ProParameter   *param, 
                                      ProParamvalue  *proval )
											throw(isis::application_exception);

	extern 	ProError isis_ProParameterScaledvalueSet(ProParameter   *param, 
                                            ProParamvalue  *proval,
                                            ProUnititem    *units)
										throw(isis::application_exception);

	 extern	ProError isis_ProUnitInit(	ProMdl        mdl,
										const ProName       unit_name,
										ProUnititem*  unit)
										throw(isis::application_exception);

	 extern	ProError isis_ProSolidMassPropertyGet(ProSolid solid,
                                          const ProName  csys_name,
                                          ProMassProperty* mass_prop )
										throw(isis::application_exception);

	 // WARNING - isis_ProDirectoryChange has an unexpected behavior. After the initial call to 
	 // isis_ProDirectoryChange, isis_ProDirectoryChange appears to access the address of path 
	 // used in the previous call.  Therefore, the same address for path must be used between calls, \
	 // or at least, the previously used path address must still be valid.
	 extern	ProError isis_ProDirectoryChange( ProPath path )
										throw(isis::application_exception);


	 extern ProError isis_ProDirectoryCurrentGet( ProPath path )
										throw(isis::application_exception);

	 extern ProError isis_ProAsmcomppathTrfGet( ProAsmcomppath *p_path,
												ProBoolean   bottom_up,
												ProMatrix    transformation )
											throw(isis::application_exception);

	 extern ProError isis_ProMdlPrincipalunitsystemGet( ProMdl          mdl,
												ProUnitsystem* principal_system)
											throw(isis::application_exception);

	 extern ProError isis_ProUnitsystemUnitGet( ProUnitsystem* system,
												ProUnitType     type,
												ProUnititem*   unit )
												throw(isis::application_exception);

	 extern	ProError isis_ProWstringCopy( wchar_t* source, wchar_t* target, int num_chars )
											throw(isis::application_exception);

	 extern ProError isis_ProElementWstringSet( ProElement element, wchar_t* value)
													throw(isis::application_exception);

	 extern  ProError isis_ProMaterialCreate(	ProSolid			part,
												const ProName				matl_name,
												ProMaterialdata		*p_matl_data,
												ProMaterial			*p_material )
											throw(isis::application_exception);

	 extern ProError isis_ProMaterialCurrentGet( ProSolid part,  ProMaterial  *p_material )
											throw(isis::application_exception);

	 extern	ProError isis_ProMaterialDelete( ProMaterial      *p_material )
											throw(isis::application_exception);

	 extern ProError isis_ProMaterialCurrentSet( ProMaterial  *p_material )
											throw(isis::application_exception);

	 extern ProError isis_ProPartMaterialsGet(	ProSolid         part,
												ProName      **p_matl_names_arr )
											throw(isis::application_exception);

	 extern ProError isis_ProMaterialPropertyGet(	ProMaterialItem *p_material,
											ProMaterialPropertyType prop_type,
											ProParamvalue    *p_value,
											ProUnititem      *p_units)
											throw(isis::application_exception);

	 extern ProError isis_ProMaterialPropertySet(	ProMaterialItem *p_material,
											ProMaterialPropertyType prop_type,
											ProParamvalue    *p_value,
											ProUnititem      *p_units)
											throw(isis::application_exception);

	 extern ProError isis_ProFemmeshExport( ProSolid			pro_solid,
											ProFemmeshData*		p_mesh_data,
											ProPath				file_name   )
											throw(isis::application_exception);

	 extern ProError isis_ProPointInit( ProSolid   owner_handle,
										int        point_id,
										ProPoint  *p_handle )
											throw(isis::application_exception);

	 extern	ProError isis_ProPointCoordGet( ProPoint    point,
											ProVector   xyz_point   )
											throw(isis::application_exception);


	 extern ProError isis_ProPntTrfEval(	ProVector   in_point,
											ProMatrix   trf,
											ProVector   out_point  )
											throw(isis::application_exception);

	extern ProError isis_ProOutputFileWrite(	ProMdl      model,
												const ProFileName name,
												ProImportExportFile   file_type,
												ProAppData  arg1,
												ProAppData  arg2,
												ProAppData  arg3,
												ProAppData  arg4 )
											throw(isis::application_exception);

	extern ProError isis_ProIntf3DFileWrite(	ProSolid solid,
        								ProIntf3DExportType file_type,
										ProPath output_file,
        								ProOutputAssemblyConfiguration configuration,
										ProSelection  reference_csys, 
        								ProOutputBrepRepresentation brep_representation, 
										ProOutputInclusion inclusion, 
        								ProOutputLayerOptions layer_options )
											throw(isis::application_exception);


	extern ProError isis_ProConfigoptSet(	const ProName option,
											ProPath option_value  )
											throw(isis::application_exception);
	

	extern ProError isis_ProRasterFileWrite( int            window_id,
                                      ProRasterDepth depth,
                                      double         width,
                                      double         height,
                                      ProDotsPerInch dots_per_inch,
                                      ProRasterType  type,
                                      ProPath        output_file )
											throw(isis::application_exception);

	extern 	ProError isis_ProWindowCurrentGet( int* p_window_id )
											throw(isis::application_exception);

	extern ProError isis_ProSolidOutlineGet(	ProSolid  p_solid,
												Pro3dPnt  r_outline_points[2])
											throw(isis::application_exception);	

	extern ProError isis_ProSolidOutlineCompute(	ProSolid              p_solid, 
                                            ProMatrix             matrix,
                                            ProSolidOutlExclTypes excludes[],
                                            int                   num_excludes,
                                            Pro3dPnt r_outline_points[2])
											throw(isis::application_exception);

	extern ProError isis_ProAsmcompPositionSet(	ProAsmcomppath* component_path,
											ProAsmcomp*     component,
											ProMatrix      position )
											throw(isis::application_exception);

	extern ProError isis_ProFeatureDelete(	ProSolid    solid,
									int        *feat_ids,
									int         feat_count,
									ProFeatureDeleteOptions *delete_opts,
									int         num_opts)
											throw(isis::application_exception);

	extern ProError isis_ProFeatureWithoptionsDelete(	ProSolid                 solid,
                                    int                     *feat_list,
                                    ProFeatureDeleteOptions *opts,
                                    int                      flags)
											throw(isis::application_exception);

	extern ProError isis_ProMdlNameGet(ProMdl     handle, 
                                       ProName    name)
											throw(isis::application_exception);

	extern ProError isis_ProMdlTypeGet(ProMdl model, 
                                ProMdlType* p_type)
											throw(isis::application_exception);

	extern ProError isis_ProMdlCopy(ProMdl      handle,
                             ProName     new_name,
                             ProMdl     *p_new_handle)
											throw(isis::application_exception);

	extern ProError isis_ProElementFree(ProElement     *p_elem )
											throw(isis::application_exception);

	extern ProError isis_ProDtmcsysTransformfileRead( wchar_t* file_name, ProElement elem_tree )
											          throw(isis::application_exception);

	extern ProError isis_ProSelect( char              option[],
			   int               max_count,
			   ProSelection     *p_in_sel,
			   ProSelFunctions  *sel_func,
			   ProSelectionEnv   sel_env,
			   ProSelAppAction   appl_act_data,
			   ProSelection    **p_sel_array,
			   int              *p_n_sels  )
											   throw(isis::application_exception);


	extern ProError isis_ProCsysIdGet( ProCsys csys, int *p_id );

	extern ProError isis_ProAxisIdGet( ProAxis  axis, int *p_id ) throw(isis::application_exception);

	extern ProError isis_ProIntfimportModelWithOptionsCreate( 
													  ProPath  import_file, 
                                                      ProPath  profile, 
                                                      ProIntfImportType type, 
                                                      ProMdlType create_type, 
                                                      ProImportRepType rep_type, 
                                                      const ProName  new_model_name, 
                                                      ProIntfimportLayerFilter filter_func, 
                                                      ProAppData application_data, 
                                                      ProMdl* created_model ) throw(isis::application_exception);


	extern ProError isis_ProFeatureStatusGet(	ProFeature    *p_feat_handle,
										ProFeatStatus *p_status) throw(isis::application_exception);


	extern	ProError isis_ProFeatureStatusGet(	ProFeature    *p_feat_handle,
										ProFeatStatus *p_status) throw(isis::application_exception);
	// Works only on assemblies
	extern  ProError isis_ProSimprepActivate( ProSolid    p_solid, 
                                    ProSimprep *p_simp_rep) throw(isis::application_exception);

	extern ProError isis_ProAssemblySimprepRetrieve(	ProFamilyName    assem_name, 
														ProName          simp_rep_name,
														ProSimprepdata  *simp_rep_data,
														ProAssembly     *p_assem) throw(isis::application_exception);

	// Works only on parts
	extern ProError isis_ProPartSimprepRetrieve(	ProFamilyName   part_name, 
													ProSimprepType  type,
													ProName         simp_rep_name,
													ProPart        *p_part) throw(isis::application_exception);

	extern ProError isis_ProSimprepdataGet(	ProSimprep			*p_simp_rep,
										ProSimprepdata		**p_data ) throw(isis::application_exception);

	extern ProError isis_ProSimprepdataFree(	ProSimprepdata   **p_data ) throw(isis::application_exception);

	extern ProError isis_ProSimprepdataitemsVisit(	ProSimprepdata *p_data, 
                                           ProFunction     filter,
                                           ProFunction     action,
                                           ProAppData      app_data ) throw(isis::application_exception);

	extern ProError isis_ProFitGlobalinterferenceCompute(	ProAssembly assem,
															ProFitComputeSetup comp_setup,
															ProBoolean set_facets,  // The options to include facets.
															ProBoolean set_quilts,  //  The options to include quilts
															ProBoolean fast_calculation, 
															ProInterferenceInfo **interf_info) throw(isis::application_exception);

	extern ProError isis_ProInterferenceInfoProarrayFree(	ProInterferenceInfo *interf_info ) throw(isis::application_exception);

	extern ProError isis_ProFitInterferencevolumeCompute(    ProInterferenceData interf_data,
													  double *volume ) throw(isis::application_exception);

	extern ProError isis_ProSelectionAsmcomppathGet(	ProSelection	selection,
														ProAsmcomppath	*p_cmp_path ) throw(isis::application_exception);

	extern ProError isis_ProSelectionModelitemGet(	ProSelection selection,
											ProModelitem *p_mdl_item) throw(isis::application_exception);

	extern ProError isis_ProElemtreeWrite(		ProElement         elemtree, 
										ProElemtreeFormat  format,
										ProPath            output_file ) throw(isis::application_exception);

	extern ProError isis_ProEngineerReleaseNumericversionGet(	int* ver ) throw(isis::application_exception);

	extern ProError isis_ProSurfaceInit (	ProMdl        p_owner_handle,
											int           surf_id,
											ProSurface   *p_surface)
																throw(isis::application_exception);			


	extern ProError isis_ProPoint3dOnsurfaceFind(	ProPoint3d      xyz,        
													ProSurface      surface,   
													ProBoolean     *on_surf,  
													ProPoint3d      closest_pt)
																throw(isis::application_exception);				

	extern ProError isis_ProModelitemMdlGet (		ProModelitem    *p_model_item, 
													ProMdl          *p_owner) 
																throw(isis::application_exception);	

	// Note - The following function is from UtilCollect.c
	extern ProError isis_ProUtilCollectSolidSurfaces (	ProSolid	    p_solid,		//	In:  The handle to the solid */
														ProSurface	    **p_surfaces	//	Out: ProArray with collected surfaces. 
																						//	The function allocates memory 
																						//	for this argument, but you must 
																						//	free it. To free the memory, 
																						//	call the function ProArrayFree()*/
																				)
																		throw (isis::application_exception);

	extern ProError  isis_ProSectionNameGet (ProSection section, ProName r_name) throw (isis::application_exception);
	extern ProError isis_ProSectionNameSet (ProSection section, wchar_t new_name[]) throw (isis::application_exception);

	extern ProError isis_ProSectionEntityFromProjection (ProSection section, ProSelection ref_geometry, int	*r_ent_id) 
																						throw (isis::application_exception);

	extern ProError isis_ProSectionEntityGet (	ProSection section, 
												int ent_id,
												Pro2dEntdef **POutEnt) throw (isis::application_exception);

	extern ProError isis_ProSectionLocationGet (	ProSection section,
											ProMatrix  location_matrix) throw (isis::application_exception);

	extern ProError isis_ProSectionEntityAdd ( ProSection section,
										Pro2dEntdef *entity2d,
										int *r_ent_id) throw (isis::application_exception);

	extern ProError isis_ProSectionAutodim ( ProSection section ) throw (isis::application_exception);

	extern ProError isis_ProSectionSolve ( ProSection section) throw (isis::application_exception);

	extern ProError isis_ProSectionRegenerate ( ProSection section) throw (isis::application_exception);


	// Need Toolkit Mechanica license
	/*
	extern	ProError isis_ProMechanicaEnter(	 ProSolid         solid,
										ProMechanicaMode mode,
										ProBool          fem )
										throw(isis::application_exception);

	// Need Toolkit Mechanica license
	extern	ProError isis_ProMechgeomrefPathGet(	ProMechGeomref entity,
													ProAsmcomppath* path)
										throw(isis::application_exception);
	
	*/
}

#endif  /* PTK_FUNCTIONS_H */