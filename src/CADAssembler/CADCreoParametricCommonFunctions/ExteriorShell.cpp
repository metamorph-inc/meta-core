
#define TRACE_ITERATION on

#include "ExteriorShell.h"
#include <ProToolkit.h>

#include <math.h>
#include <UtilPtc.h>
#include <UtilTree.h>
#include <FeatureFactory.h>
#include <ProDtmPln.h>
#include <ProSolidify.h>
#include <ProFeatType.h>
#include <ProItemerr.h>
#include <ProXsec.h>
#include <UtilMath.h>

#define PLATFORM X86E_WIN64

#include <ProUtil.h>
#include <ProObjects.h>
#include <ProShrinkwrap.h>
#include <ProMode.h>
#include <ProMdl.h>
#include <ProAssembly.h>
#include <ProToolkitErrors.h>
#include <ProWindows.h>
#include <ProAnalysis.h>
#include <ProSurface.h>

#include <boost/math/special_functions/sign.hpp>
#include <boost/make_shared.hpp>

#include <Selector.h>
#include <TemplateModel.h>


namespace cf = isis::creo::feature;

namespace isis {
namespace hydrostatic {

ProError select_cutting_plane_filter (ProGeomitem* geom_item, ProAppData  app_data) 
{
	
	ProSurface* surface = reinterpret_cast<ProSurface*>(app_data);

	ProError rc;
	ProSrftype surface_type;
	switch( rc = ProGeomitemToSurface (geom_item, surface) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "no surface on geometry";
		return PRO_TK_CONTINUE;
	}
	switch( rc = ProSurfaceTypeGet (*surface, &surface_type) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "surface type unknown";
		return PRO_TK_CONTINUE;
	}
	switch(surface_type) {
	case PRO_SRF_PLANE: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "could not get surface for cut plane";
		return PRO_TK_CONTINUE;
	}
	int plane_id;
	switch( rc = ProSurfaceIdGet (*surface, &plane_id) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "not a surface";
		return PRO_TK_CONTINUE;
	}
	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "cut plane: "
		<< " owner = " << geom_item->owner
		<< " id = " << plane_id;
	return PRO_TK_NO_ERROR;
}

ProError select_cutting_plane_id_action (ProGeomitem* geom_item, 
	ProError status, ProAppData  app_data) 
{
	switch(status) {
	case PRO_TK_NO_ERROR:
	case PRO_TK_CONTINUE:
		return PRO_TK_NO_ERROR;
	default:
		return PRO_TK_E_NOT_FOUND;
	}
	return PRO_TK_E_NOT_FOUND;
}


ProError select_cutting_plane_id_filter (ProGeomitem* geom_item, ProAppData  app_data) 
{
	
	int* plane_id = reinterpret_cast<int*>(app_data);

	ProError rc;
	ProSurface surface;
	ProSrftype surface_type;
	switch( rc = ProGeomitemToSurface (geom_item, &surface) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "no surface on geometry";
		return PRO_TK_CONTINUE;
	}
	switch( rc = ProSurfaceTypeGet (surface, &surface_type) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "surface type unknown";
		return PRO_TK_CONTINUE;
	}
	switch(surface_type) {
	case PRO_SRF_PLANE: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "could not get surface for cut plane";
		return PRO_TK_CONTINUE;
	}
	switch( rc = ProSurfaceIdGet (surface, plane_id) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "not a surface";
		return PRO_TK_CONTINUE;
	}
	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "cut plane owner = " << geom_item->owner;
	return PRO_TK_NO_ERROR;
}

			
std::ostream& operator<< (std::ostream& out_stream, const Result& result) {
	out_stream 
		<< "result: "
		<< " depth = " << result.depth 
		<< " volume = " << result.volume 
		<< " cob = " << result.cob 
		<< " wetted_area = " << result.wetted_area 
		<< " wetted_frontal_area = " << result.projected_frontal_area 
		<< std::endl;
	out_stream << " cross_section_area:"
		<< " size = " << result.xsection_area.size()
		<< " values = ";
	for each( std::pair<double,double> value in result.xsection_area ) {
		out_stream << " " << value.first << ":" << value.second;
	}
	out_stream << std::endl;
	return out_stream;
}

/** */
ExteriorShell::ExteriorShell(const ::std::string name) 
		:// m_log_f(isis_FILE_CHANNEL),
		//m_log_cf(isis_FILE_AND_CONSOLE_CHANNEL),
		m_name(name), m_path(), m_working_solid(NULL), m_wrapped_solid(NULL),
		m_long_orient(AXIS_X_POSITIVE), m_vert_orient(AXIS_Y_POSITIVE),
		m_depth_axis(), m_heel_axis(), m_trim_axis(),
		m_retain_datums_flag(false)
{ }


void dump_error_list( std::ostream& out_stream, ProErrorlist errors) {
	out_stream << "[" << errors.error_number << std::endl;
	for( int ix=0; ix < errors.error_number; ++ix) {
		ProItemerror item = errors.error_list[ix];  
		out_stream << "error: " << item.err_item_id
			<< " type: " << item.err_item_type 
			<< " : " << item.error << std::endl;
	}
}
		
/**
make and export a shell solid part.
The shell is a mostly convex hull around the assembly
*/
ProError ExteriorShell::create_shrinkwrap( std::string in_name ) {
	ProError status;
	char pro_str[128];
	ProShrinkwrapOptions options;

	switch(status = ProShrinkwrapoptionsAlloc (PRO_SWCREATE_FACETED_SOLID, &options) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not preallocate the options" ;
		return PRO_TK_GENERAL_ERROR;
	}
	switch(status = ProShrinkwrapoptionsAutoholefillingSet (options, PRO_B_TRUE) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not preallocate the auto-hole-filling option" ;
		return PRO_TK_GENERAL_ERROR;
	}
	switch( status = ProShrinkwrapoptionsQualitySet(options, 2 ) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not set quality option" ;
		return PRO_TK_GENERAL_ERROR;
	}

	switch( status = ProShrinkwrapoptionsIgnoresmallsurfsSet( options, PRO_B_FALSE, 20.0) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not set ignore-smaller-surfaces option" ;
		return PRO_TK_GENERAL_ERROR;
	}

	switch( status = ProShrinkwrapoptionsFacetedformatSet (options, PRO_SWFACETED_PART) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not set faceted-format option" ;
		return PRO_TK_GENERAL_ERROR;
	}
	switch( status = ProShrinkwrapoptionsAssignmasspropsSet(options, PRO_B_FALSE) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not set assign-mass-props option" ;
		return PRO_TK_GENERAL_ERROR;
	}

	ProMdl model = static_cast<ProMdl>(m_working_solid);
	ProMdlType type;
	switch( status = ProMdlTypeGet( model, &type ) ) {
	case PRO_TK_NO_ERROR: break;
	case PRO_TK_BAD_INPUTS:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "the input argument is invalid: model = " << model;
		return PRO_TK_GENERAL_ERROR;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not obtain model type: status = " << status ;
		return PRO_TK_GENERAL_ERROR;
	}
	switch( type ) {
	case PRO_MDL_PART:
	case PRO_MDL_ASSEMBLY:
		break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "not an appropriate model " << type ;
		return PRO_TK_GENERAL_ERROR;
	}
	ProSolid solid = static_cast<ProSolid>(model);

	ProName name;
	ProStringToWstring(name, const_cast<char*>(in_name.c_str()) );

	switch( status = ProSolidCreate(name, PRO_PART, &m_wrapped_solid) ) {
	case PRO_TK_NO_ERROR: break;
	case PRO_TK_BAD_INPUTS:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) 
			<< "one or more of the input arguments are invalid. "
			<< ProWstringToString(pro_str, name);
		break;
	case PRO_TK_E_FOUND:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR)
			<< "an object of the specified name and type already exists: "
			<< ProWstringToString(pro_str, name);
		break;
	case PRO_TK_GENERAL_ERROR:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR)
			<< "the object could not be created, generally.";
		break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not create a new solid = "
			<< status;
		return PRO_TK_GENERAL_ERROR;
	}
	isis::creo::model::make_solid_templated( solid, m_wrapped_solid );

	switch( status = ProSolidShrinkwrapCreate (solid, m_wrapped_solid, NULL, options)) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not create and export the shrinkwrap file" ;
		return PRO_TK_GENERAL_ERROR;
	}

	switch( status = ProShrinkwrapoptionsFree (options)) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not free the options" ;
		return PRO_TK_GENERAL_ERROR;
	}

	isis_LOG(lg, isis_FILE, isis_INFO) << "successfully created shrinkwrap part: "
		<< name ;

	return PRO_TK_NO_ERROR;
}
		

/** 
specify another displacement anchor point
*/
ProError ExteriorShell::addDepth( const depth_type in_depth ) {
	m_depth_axis.push_back( in_depth );
	return PRO_TK_NO_ERROR;
}

ProError ExteriorShell::addHeelAngle( const angle_type in_heel_angle ) {
	m_heel_axis.push_back( in_heel_angle );
	return PRO_TK_NO_ERROR;
}

ProError ExteriorShell::addTrimAngle( const angle_type in_trim_angle ) {
	m_trim_axis.push_back( in_trim_angle );
	return PRO_TK_NO_ERROR;
}

/**
The tool used for cutting the body at the waterline.
*/


class Cutter {
private:

	ProSolid m_total_solid;
	ProModelitem m_total_model_item;
	ProSelection m_total_selection;
	ProMdlType m_total_model_type;
	ProMode m_mode;

	isis::hydrostatic::bounding_box_type m_bounds;
    double m_total_upper_depth; // mm
    double m_total_lower_depth; // mm

	double m_total_volume; // mm^3
	double* m_total_cob;
	ProMassProperty m_total_mass_prop;

	std::vector<int> m_feat_ids;

	ProFeature m_csys_feature;
	ProFeature m_cob_pnt_feature;
	ProFeature m_x_pln_feature;
	ProFeature m_y_pln_feature;
	ProFeature m_z_pln_feature;
	ProFeature m_trim_axis_feature;
	ProFeature m_trim_pln_feature;
	ProFeature m_heel_axis_feature;
	ProFeature m_heel_pln_feature;
	ProFeature m_depth_pln_feature;
	ProFeature m_truncate_sld_feature;

	Orientation m_long_orient;
	Orientation m_vert_orient;
	
public:

	bool m_debug_retain_datums;

	Cutter(ProSolid in_solid, Orientation in_long, Orientation in_vert) 
		: //m_log_f(isis_FILE_CHANNEL),
		  //m_log_cf(isis_FILE_AND_CONSOLE_CHANNEL),
		  m_total_solid(in_solid), m_debug_retain_datums(false),
		  m_long_orient(in_long), m_vert_orient(in_vert) 
	 {
		 if (m_long_orient == m_vert_orient) {
			 isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "inconsistent orientations" ;
			 throw std::runtime_error("inconsistent orientations");
		 }
		ProError status;
		const int main_dim = 2; // {0, 1, 2}

		switch( status = ProMdlToModelitem(m_total_solid, &m_total_model_item) ) {
		case PRO_TK_NO_ERROR: break;
		default:  
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "no currently active model item" ;
			return; // PRO_TK_GENERAL_ERROR; 
		} 

		switch( status = ProSelectionAlloc(NULL, &m_total_model_item, &m_total_selection) ) {
		case PRO_TK_NO_ERROR: break;
		default: 
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "no top level selection obtained" ;
			return; // PRO_TK_GENERAL_ERROR; 
		}

		switch( status = ProMdlTypeGet(m_total_solid, &m_total_model_type) ) {
		case PRO_TK_NO_ERROR: break;
		default:  
			return; // PRO_TK_GENERAL_ERROR; 
		} 
		switch( m_total_model_type ) {
		case PRO_MDL_ASSEMBLY:
		case PRO_MDL_PART:
			break;
		default:
			return; // PRO_TK_GENERAL_ERROR; 
		} 

		ProMatrix  transform = {
			{1.0, 0.0, 0.0, 0.0},
			{0.0, 1.0, 0.0, 0.0},
			{0.0, 0.0, 1.0, 0.0},
			{0.0, 0.0, 0.0, 1.0} };

		int num_excludes = 3;
		ProSolidOutlExclTypes excludes[] = {
			PRO_OUTL_EXC_DATUM_PLANE,
			PRO_OUTL_EXC_DATUM_POINT,
			PRO_OUTL_EXC_DATUM_CSYS};

		switch( status = ProSolidOutlineCompute(m_total_solid, transform, 
			excludes, num_excludes, m_bounds.points) ) 
		{
		case PRO_TK_NO_ERROR: break;
		default:
			return; // PRO_TK_GENERAL_ERROR; 
		} 
		m_total_upper_depth = std::sqrt( 
			std::pow(m_bounds.points[1][0], 2) +
			std::pow(m_bounds.points[1][1], 2) +
			std::pow(m_bounds.points[1][2], 2) ) ; 

		m_total_lower_depth = - std::sqrt( 
			std::pow(m_bounds.points[0][0], 2) +
			std::pow(m_bounds.points[0][1], 2) +
			std::pow(m_bounds.points[0][2], 2) ) ; 
					

		{
			wchar_t* default_csys_name = NULL;
			switch( status = ProSolidMassPropertyGet(m_total_solid, default_csys_name, &m_total_mass_prop) ) {
	
			case PRO_TK_NO_ERROR: break;
			case PRO_TK_BAD_INPUTS:
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "the solid handle is invalid " ;
				return; // PRO_TK_GENERAL_ERROR; 
			case PRO_TK_E_NOT_FOUND: 
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "the specified coordinate system was not found. " ;
				return; // PRO_TK_GENERAL_ERROR; 
			case PRO_TK_GENERAL_ERROR: 
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "a general error occurred and the function failed. " ;
				return; // PRO_TK_GENERAL_ERROR; 
			default:
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not extract the mass property " << status ;
				return; // PRO_TK_GENERAL_ERROR; 
			} 
			m_total_volume = m_total_mass_prop.volume;
			m_total_cob = m_total_mass_prop.center_of_gravity; 
			isis_LOG(lg, isis_FILE, isis_INFO) << "total-volume: " << m_total_volume ;
		}
			
		cf::create::Csys_default( m_csys_feature, m_total_solid, m_total_selection, "DEFAULT_CSYS" );
		m_feat_ids.push_back(m_csys_feature.id);
		isis_LOG(lg, isis_FILE, isis_INFO) << "created global coordinate system feature = " << m_csys_feature.id ;

		cf::create::Point( m_cob_pnt_feature, m_total_solid, m_total_selection, "COB_DTM_PNT",
			m_csys_feature, m_total_cob );
		m_feat_ids.push_back(m_cob_pnt_feature.id);
		isis_LOG(lg, isis_FILE, isis_INFO) << "created center-of-bouyancy feature = " << m_cob_pnt_feature.id;
			
		cf::create::Plane( m_x_pln_feature, m_total_solid, m_total_selection, "X_DTM_PLN", PRO_DTMPLN_DEF_X );
		m_feat_ids.push_back(m_x_pln_feature.id);

		cf::create::Plane( m_y_pln_feature, m_total_solid, m_total_selection, "Y_DTM_PLN", PRO_DTMPLN_DEF_Y );
		m_feat_ids.push_back(m_y_pln_feature.id);

		cf::create::Plane( m_z_pln_feature, m_total_solid, m_total_selection, "Z_DTM_PLN", PRO_DTMPLN_DEF_Z );
		m_feat_ids.push_back(m_z_pln_feature.id);
		isis_LOG(lg, isis_FILE, isis_INFO) << "created global coordinate datum plane features: "
			<< " x = " << m_x_pln_feature.id 
			<< " y = " << m_y_pln_feature.id 
			<< " z = " << m_z_pln_feature.id;

		ProFeature* keel_pln = NULL;
		ProFeature* horiz_pln = NULL;
		ProFeature* cross_pln = NULL;
		{
			switch( m_long_orient ) {
			case AXIS_X_POSITIVE:
			case AXIS_X_NEGATIVE:
				cross_pln = &m_x_pln_feature;
				break;
			case AXIS_Y_POSITIVE:
			case AXIS_Y_NEGATIVE:
				cross_pln = &m_y_pln_feature;
				break;
			case AXIS_Z_POSITIVE:
			case AXIS_Z_NEGATIVE:
				cross_pln = &m_z_pln_feature;
				break;
			default:
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "inconsistent axies" ;
			}

			switch( m_vert_orient ) {
			case AXIS_X_POSITIVE:
			case AXIS_X_NEGATIVE:
				horiz_pln = &m_x_pln_feature;
				break;
			case AXIS_Y_POSITIVE:
			case AXIS_Y_NEGATIVE:
				horiz_pln = &m_y_pln_feature;
				break;
			case AXIS_Z_POSITIVE:
			case AXIS_Z_NEGATIVE:
				horiz_pln = &m_z_pln_feature;
				break;
			default:
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "inconsistent axies" ;
			}
			// the cross plane is the unused plane
			if (cross_pln == &m_x_pln_feature) {
				keel_pln = (horiz_pln == &m_y_pln_feature) 
					? &m_z_pln_feature : &m_y_pln_feature;
			} else if (cross_pln == &m_y_pln_feature) {
				keel_pln = (horiz_pln == &m_x_pln_feature) 
					? &m_z_pln_feature : &m_x_pln_feature;
			} else  {
				keel_pln = (horiz_pln == &m_x_pln_feature) 
					? &m_y_pln_feature : &m_x_pln_feature;
			}
		}
		ProDtmplnFlipDir trim_flip = PRO_DTMPLN_FLIP_DIR_NO;
		ProDtmplnFlipDir heel_flip = PRO_DTMPLN_FLIP_DIR_NO;
		ProDtmplnFlipDir depth_flip = PRO_DTMPLN_FLIP_DIR_NO;

		cf::create::Axis( m_trim_axis_feature, m_total_solid, m_total_selection, 
			"TRIM_DTM_AXIS", *cross_pln, *horiz_pln );
		m_feat_ids.push_back(m_trim_axis_feature.id);
		isis_LOG(lg, isis_FILE, isis_INFO) << "created trim datum axis feature = " << m_trim_axis_feature.id;

		cf::create::Plane_Pivot( m_trim_pln_feature, m_total_solid, m_total_selection, 
			"TRIM_DTM_PLN", m_trim_axis_feature, *horiz_pln, 0.0 );
		m_feat_ids.push_back(m_trim_pln_feature.id);
		isis_LOG(lg, isis_FILE, isis_INFO) << "created trim datum plane feature = " << m_trim_pln_feature.id;

		cf::create::Axis( m_heel_axis_feature, m_total_solid, m_total_selection, 
			"HEEL_DTM_AXIS", m_trim_pln_feature, *keel_pln );
		m_feat_ids.push_back(m_heel_axis_feature.id);
		isis_LOG(lg, isis_FILE, isis_INFO) << "created heel datum axis feature = " << m_heel_axis_feature.id;

		cf::create::Plane_Pivot( m_heel_pln_feature, m_total_solid, m_total_selection, 
			"HEEL_DTM_PLN", m_heel_axis_feature, m_trim_pln_feature, 0.0 );
		m_feat_ids.push_back(m_heel_pln_feature.id);
		isis_LOG(lg, isis_FILE, isis_INFO) << "created heel datum plane feature = " << m_heel_pln_feature.id;

		cf::create::Plane_Offset( m_depth_pln_feature, m_total_solid, m_total_selection, 
			"DEPTH_DTM_PLN", depth_flip, m_heel_pln_feature, 0.0 );
		m_feat_ids.push_back(m_depth_pln_feature.id);
		isis_LOG(lg, isis_FILE, isis_INFO) << "created depth datum plane feature = " << m_depth_pln_feature.id;

		switch( status = ProSolidRegenerate( m_total_solid, PRO_REGEN_NO_FLAGS ) ) {
		case PRO_TK_NO_ERROR: break;
		default:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not regenerate status = " << status;
		}

		cf::create::Solidify_Truncate( m_truncate_sld_feature, m_total_solid, m_total_selection, 
			"TRUNC_SLD", m_depth_pln_feature );
		m_feat_ids.push_back(m_truncate_sld_feature.id);
		isis_LOG(lg, isis_FILE, isis_INFO) << "created submerged datum solid feature = " << m_truncate_sld_feature.id;

	}

			
	~Cutter() {
		ProError status;
		switch( status = ProSelectionFree(&m_total_selection) ) {
		case PRO_TK_NO_ERROR: break;
		}
		
		if ( ! m_debug_retain_datums ) {
			const int num_feature_delete_opts = 1;
			ProFeatureDeleteOptions feature_delete_opts[num_feature_delete_opts];

			switch( status = ProFeatureDelete(m_total_solid, &m_feat_ids[0], static_cast<long>(m_feat_ids.size()), 
				feature_delete_opts, num_feature_delete_opts) ) 
			{
			case PRO_TK_NO_ERROR: break;
			}
		}

		switch( status = ProSolidRegenerate(m_total_solid, PRO_REGEN_NO_FLAGS) ) {
		case PRO_TK_NO_ERROR: break;
		case PRO_TK_UNATTACHED_FEATS:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "unattached features were detected, "
				<< "but there was no regeneration failure" ;
			throw std::runtime_error("general TK ERROR");
		case PRO_TK_REGEN_AGAIN:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "the model is too complex to regenerate the first time" ;
			throw std::runtime_error("general TK ERROR");
      	case PRO_TK_GENERAL_ERROR:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "failure in regeneration" ;
			throw std::runtime_error("general TK ERROR");
      	case PRO_TK_BAD_INPUTS:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "incompatible flags used." ;
			throw std::runtime_error("general TK ERROR");
      	case PRO_TK_BAD_CONTEXT:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "invalid regen flags and/or combination of regeneration flags"
				<< "if mixed with PRO_REGEN_FORCE_REGEN." ;
			throw std::runtime_error("general TK ERROR");
		default:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not regen the solid" << status ;
			throw std::runtime_error("general TK ERROR");
		} 
	}

	/**
	The main part of this procedure is here.
	We are using the "Bisection method".
	It is very simple but converges somewhat slowly.
	http://en.wikipedia.org/wiki/Bisection_method
	*/
	bool bisection_method(double& f_x1, double& x1, 
				double& f_x2, double& x2, 
				double& f_x3, double& x3)
	{
		if (boost::math::sign(f_x3) == boost::math::sign(f_x1)) {
			x1 = x3;
			f_x1 = f_x3;
		} else {
			x2 = x3;
			f_x2 = f_x3;
		}
		x3 = (x1 + x2) / 2.0;
		return true;
	}

	/**
	a more efficient approach is "double false position" as found in
	http://en.wikipedia.org/wiki/False_position_method
	*/
	bool secant_method(double& f_x1, double& x1, 
				double& f_x2, double& x2, 
				double& f_x3, double& x3)
	{

		double ak, f_ak;
		double bk, f_bk;
		double ck;
		if (boost::math::sign(f_x3) == boost::math::sign(f_x1)) {
			ak = x2;
			bk = x3;
			f_ak = f_x2;
			f_bk = f_x3;
		} else {
			ak = x3;
			bk = x1;
			f_ak = f_x3;
			f_bk = f_x1;
		}
		ck = bk - ((f_bk) * (bk - ak)) / (f_bk - f_ak);

		x1 = ak; f_x1 = f_ak;
		x2 = bk; f_x2 = f_bk;
		x3 = ck;
		return true;
	}

	/**
	there is a problem with ridder's method.
	It require the evaluation of the function twice per iteration.
	The following method is modified.
	http://en.wikipedia.org/wiki/Ridders%27_method
	*/
	bool ridders_method(double& f_x1, double& x1, 
				double& f_x2, double& x2, 
				double& f_x3, double& x3)
	{
		double x4 = x3 + (x3-x1)*(boost::math::sign(f_x1-f_x2)*f_x3) / 
			std::sqrt( f_x3*f_x3 - f_x1*f_x2 );

		if (boost::math::sign(f_x3) == boost::math::sign(f_x1)) {
			x1 = x3;
			f_x1 = f_x3;
		} else {
			x2 = x3;
			f_x2 = f_x3;
		}
		x3 = x4;
		return true;
	}

	/**
	use binary method to obtain an approximation of the water-line.
	The water-line is that point where, for a given orientation, the displaced
	volume is a specified quantity.
	The binary search method is appropriate for monotonicly increasing functions.
	Clearly pushing a body deeper into an incompresible invisid fluid will never cause
	the displaced volume to decrease, so the criteria is met.

	<ol>
	<li> load the displacing body </li>
	<li> find the bounding box which contains the displacing body </li>
	<li> orient the cut plane normal relative to the body </li>
	</ol>
	*/
	ProError computeHydrostatic( Result& out_result, const double in_tolerance, 
		const double in_displacement, const double in_heel_angle, const double in_trim_angle ) 
	{
		ProError status;
		char wipName[64];
		const volume_type limit_volume = m_total_volume * in_tolerance;

		isis_LOG(lg, isis_FILE, isis_INFO) << "parameters " << '\n'
			<< "  displacement [" << in_displacement <<  "] " << '\n'
			<< "  heel-angle [" << in_heel_angle << "] " << '\n'
			<< "  trim-angle [" << in_trim_angle << "] " << '\n'
			;
		volume_type goal_volume = in_displacement;
						
		if (m_total_volume < goal_volume) {
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) 
				<< "no solution volume: total = " << m_total_volume 
				<< " < " << " goal = " << goal_volume ;
			return PRO_TK_GENERAL_ERROR;
		}

		switch( status = cf::create::Plane_Angle_adjust(m_total_selection, m_heel_pln_feature, in_heel_angle) ) {
		case PRO_TK_NO_ERROR: break;
		default:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not adjust the trim " << status ;
			return PRO_TK_GENERAL_ERROR;
		}

		switch( status = cf::create::Plane_Angle_adjust(m_total_selection, m_trim_pln_feature, in_trim_angle) ) {
		case PRO_TK_NO_ERROR: break;
		default:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not adjust the trim " << status ;
			return PRO_TK_GENERAL_ERROR;
		}

		double upper_depth = m_total_upper_depth; 
		double lower_depth = m_total_lower_depth; 

		/*
		because we are using a root finding algorithm, the
		goal value is zero (0).  This means that the goal volume
		needs to be adjusted, offset by the actual goal volume.
		*/
		double upper_volume = m_total_volume - goal_volume;
		double lower_volume = 0.0 - goal_volume;

		ProMassProperty work_mass_properties;

		double cob_depth;
		{
		    double candidate_cob_depth;
			cf::Selector work_plane(m_total_solid, m_depth_pln_feature, PRO_SURFACE);
			cf::Selector work_point(m_total_solid, m_cob_pnt_feature, PRO_POINT);

			switch( status = ProGeomitemDistanceEval(*work_point, *work_plane, &candidate_cob_depth ) ) {
			case PRO_TK_NO_ERROR: break;
			default:
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not acquire the cob depth " << status ;
			}
			switch( status = cf::create::Plane_Offset_adjust(m_total_selection, m_depth_pln_feature, candidate_cob_depth) ) {
			case PRO_TK_NO_ERROR: break;
			default:
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not adjust the depth " << status ;
			}
			cf::Selector cob_plane(m_total_solid, m_depth_pln_feature, PRO_SURFACE);
		    double test_cob_depth;
			switch( status = ProGeomitemDistanceEval(*work_point, *work_plane, &test_cob_depth ) ) {
			case PRO_TK_NO_ERROR: break;
			default:
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not acquire the test cob depth " << status ;
			}
			isis_LOG(lg, isis_FILE, isis_INFO) << "cob depth [ " << candidate_cob_depth << " : " << test_cob_depth << " ]" ;
			cob_depth = candidate_cob_depth * ((candidate_cob_depth < test_cob_depth) ? -1.0 : 1.0);
		}

		double last_successful_depth = cob_depth;
		double midpoint_depth = cob_depth;

		const int max_steps = 100;
		bool has_more_work = true;
		for ( int nx=0; has_more_work; ++nx ) {
			if (nx > max_steps) {
				isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "maximum number of steps exceeded " << nx ;
				break;
			}

#ifdef TRACE_ITERATION
			isis_LOG(lg, isis_FILE, isis_INFO) << '\n' 
				<< "search: step[" << nx << "] "<< " depth=" << midpoint_depth ;
#endif
			switch( status = cf::create::Plane_Offset_adjust(m_total_selection, m_depth_pln_feature, midpoint_depth) ) {
			case PRO_TK_NO_ERROR: break;
			default:
				isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "could not adjust the depth " << status ;
			}

			ProSolidRegenerationStatus regen_status;
			switch(status = ProSolidRegenerationstatusGet(m_total_solid, &regen_status) ) {
			case PRO_TK_NO_ERROR: break;
			case PRO_TK_BAD_INPUTS:
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "the solid handle is invalid " ;
				return PRO_TK_GENERAL_ERROR; 
			}

			switch( regen_status ) {
			case PRO_SOLID_FAILED_REGENERATION:
				isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "the cut missed (due to a regeneration error): " << midpoint_depth ;
				//********* FAILURE *****************
		        has_more_work = false;
				continue;
			case PRO_SOLID_REGENERATED:
				break;
			default:
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "the cut regeneration failed: " << regen_status ;
				return PRO_TK_GENERAL_ERROR; 
			}

			wchar_t*  default_csys_name = NULL;
			switch( status = ProSolidMassPropertyGet(m_total_solid, default_csys_name, &work_mass_properties) ) {
			case PRO_TK_NO_ERROR: break;
			case PRO_TK_BAD_INPUTS:
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "the solid handle is invalid " ;
				return PRO_TK_GENERAL_ERROR; 
			case PRO_TK_E_NOT_FOUND: 
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "the specified coordinate system was not found. " ;
				return PRO_TK_GENERAL_ERROR; 
			case PRO_TK_GENERAL_ERROR: 
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "a general error occurred and the function failed. " ;
				return PRO_TK_GENERAL_ERROR; 
			default:
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not acquire the properties of the wetted volume " << status ;
				return PRO_TK_GENERAL_ERROR; 
			} 
			double midpoint_volume = work_mass_properties.volume - goal_volume;
			if ( midpoint_volume + goal_volume + limit_volume > m_total_volume ) {
				isis_LOG(lg, isis_FILE, isis_INFO) << "the cut missed (by volume) at depth: " << midpoint_depth ;
				switch( boost::math::sign( midpoint_depth - last_successful_depth )) {
				case -1: lower_depth = midpoint_depth; 
				    midpoint_depth = (midpoint_depth + last_successful_depth) / 2.0;
					break;
				case +1: upper_depth = midpoint_depth; 
				    midpoint_depth = (midpoint_depth + last_successful_depth) / 2.0;
					break;
				default:
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "what???" ;
					midpoint_depth = 0.0 - midpoint_depth;
				}
				continue;
			}

			if (std::abs(midpoint_volume) < limit_volume ) {
				//********* SUCCESS *****************
		        has_more_work = false;
				continue;
			}
			last_successful_depth = midpoint_depth;

			bisection_method(
				upper_volume, upper_depth, 
				lower_volume, lower_depth,
				midpoint_volume, midpoint_depth); 

#ifdef TRACE_ITERATION
			isis_LOG(lg, isis_FILE, isis_INFO) << "depth:  \t[" << lower_depth  << " \t: " << midpoint_depth  << " \t: " << upper_depth  << "\t]" ;
			isis_LOG(lg, isis_FILE, isis_INFO) << "volume: \t[" << lower_volume << " \t: " << midpoint_volume << " \t: " << upper_volume << "\t]" ;
#endif
		}	

		{
			out_result.total_volume = this->m_total_volume;
		}
		// extract results for the target displacement:heel:trim values 
		{ // volume 
			out_result.volume = work_mass_properties.volume;
			isis_LOG(lg, isis_FILE, isis_INFO) << "volume: " << out_result.volume << " goal=" << goal_volume ;	
		}
		{ // target depth
			isis_LOG(lg, isis_FILE, isis_INFO) << "depth: " << midpoint_depth ;					
			out_result.depth = midpoint_depth;
		}
		{ // center of buoyancy
			double* cob_pfc = work_mass_properties.center_of_gravity;

			out_result.cob(0) = cob_pfc[0];
			out_result.cob(1) = cob_pfc[1];
			out_result.cob(2) = cob_pfc[2];
			isis_LOG(lg, isis_FILE, isis_INFO) << "cob: " << out_result.cob ;
		}
		Pro3dPnt points[2];
		{
			ProMatrix  transform = {
				{1.0, 0.0, 0.0, 0.0},
				{0.0, 1.0, 0.0, 0.0},
				{0.0, 0.0, 1.0, 0.0},
				{0.0, 0.0, 0.0, 1.0} };

			int num_excludes = 3;
			ProSolidOutlExclTypes excludes[] = {
				PRO_OUTL_EXC_DATUM_PLANE,
				PRO_OUTL_EXC_DATUM_POINT,
				PRO_OUTL_EXC_DATUM_CSYS};

			switch( status = ProSolidOutlineCompute(m_total_solid, transform, 
				excludes, num_excludes, points) ) 
			{
			case PRO_TK_NO_ERROR: break;
			default:
			    isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not compute the solid outline: " ;
			} 
		}
		{ // wetted area
			/*  cannot use the datum to cut a section as it lies precisely on the surface
			ProSurface cut_surface;
			switch( status = ProFeatureGeomitemVisit(&m_depth_pln_feature, PRO_SURFACE, 
				select_cutting_plane_id_action, select_cutting_plane_id_filter,
				select_geom_data) ) 
			{
			case PRO_TK_NO_ERROR: break;
			case PRO_TK_BAD_INPUTS:
				isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "one or more arguments was invalid.";
				break;
			default:
				isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) 
					<< "the action function returned a value other than PRO_TK_NO_ERROR and visiting stopped. " 
					<< status;
			} 
			double cut_area;
			switch( status = ProSurfaceAreaEval(cut_surface, &cut_area) ) {
			case PRO_TK_NO_ERROR: break;
			default:
				isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) 
					<< "the surface area could not be determined. " 
					<< status;
			}
			*/
			double cut_area = 
				(points[1][0] - points[0][0]) * (points[1][1] - points[0][1]); 
			out_result.wetted_area = work_mass_properties.surface_area - cut_area;
		}
		{ // frontal projection of wetted area
			out_result.projected_frontal_area = 
				(points[1][0] - points[0][0]) * (points[1][2] - points[0][2]); 
		}
		{ // transverse cross section areas
			ProSolid solid_owner = m_total_solid;
			ProName xsec_name = L"hydraulic_reference_area";
			int plane_id;
			{
				ProAppData select_geom_data = static_cast<ProAppData>(&plane_id);
				switch( status = ProFeatureGeomitemVisit(&m_y_pln_feature, PRO_SURFACE, 
					select_cutting_plane_id_action, select_cutting_plane_id_filter,
					select_geom_data) ) 
				{
				case PRO_TK_NO_ERROR: break;
				case PRO_TK_BAD_INPUTS:
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "one or more arguments was invalid.";
					break;
				default:
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) 
						<< "the action function returned a value other than PRO_TK_NO_ERROR and visiting stopped. " 
						<< status;
				} 
			}

			const int step_count = 10;
			double start_distance = points[0][1];
			double finish_distance = points[1][1];
			double step_size = (finish_distance - start_distance) / (step_count + 1);

			out_result.xsection_area.push_back( 
				std::pair<double,double>(start_distance, 0.0) );

			for( int ix=0; ix < step_count; ++ix ) {
				double cut_distance = start_distance + step_size * (ix+1);
				ProXsec xsec;
				ProDimension dimension;
				switch( status = ProXsecParallelCreate(solid_owner, xsec_name, 
					plane_id, cut_distance, &xsec, &dimension) ) 
				{
				case PRO_TK_NO_ERROR: break;
				case PRO_TK_E_FOUND: 
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) 
						<< "cross section already exists with name = " 
						<< ProWstringToString(wipName, xsec_name);
					break;
				case PRO_TK_BAD_INPUTS:
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) 
						<< "invalid input parameter(s) to create parallel cross section: "
						<< " owner = " << solid_owner
						<< " section-name = " << xsec_name
						<< " plane-id = " << plane_id;
					break;
				case PRO_TK_GENERAL_ERROR:
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "failed to create cross section.";
					break;
				default:
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not create the un-wetted x-section: " << status;
				}

				ProName csys_name = L"DEFAULT_CSYS";
				ProMassProperty xsec_prop;
				switch( status = ProXsecMassPropertyCompute(&xsec, csys_name, &xsec_prop) ) {
				case PRO_TK_NO_ERROR: break;
				case PRO_TK_BAD_INPUTS:
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "invalid input parameter(s) for mass prop: "
						<< " csys-name = " << ProWstringToString(wipName, csys_name);
					break;
				case PRO_TK_GENERAL_ERROR:
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "failed to compute cross section.";
					break;
				default:
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not compute the un-wetted x-section: " << status;
				}
				out_result.xsection_area.push_back( 
					std::pair<double,double>(cut_distance, xsec_prop.surface_area) );

				{ // clean up the x-section
					// ProFeature xfeat;
					// switch( status = ProXsecFeatureGet(&xsec, &xfeat) ) {
					// case PRO_TK_NO_ERROR: break;
					// default:
					// 	isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not get the un_wetted x-feature: " << status;
					// }
					switch( status = ProXsecDelete( &xsec ) ) {
					case PRO_TK_NO_ERROR: break;
					default:
						isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not delete the un_wetted x-section: " << status;
					}
					// const int num_feature_delete_opts = 1;
					// ProFeatureDeleteOptions feature_delete_opts[num_feature_delete_opts]; 
					// int feat_ids[] = { xfeat.id };
					// switch( status = ProFeatureDelete(m_total_solid, feat_ids, 1,
					// feature_delete_opts, num_feature_delete_opts) ) 
					// {
					// case PRO_TK_NO_ERROR: break;
					// case PRO_TK_BAD_INPUTS:
					// 	isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "bad inputs to delete the un_wetted x-feature" 
					// 		<< feat_ids;
					// 	break;
					// case PRO_TK_GENERAL_ERROR:
					// 	isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "general error deleting the un_wetted x-feature" 
					// 	break;
					// default:
					// 	isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not delete the un_wetted x-feature: " << status;
					// }
				}
			}
			out_result.xsection_area.push_back( 
				std::pair<double,double>(finish_distance, 0.0) );
		}

		return PRO_TK_NO_ERROR;
	}

	/**
	Compute a space of results.
	*/
	ProError computeHydrostatic( PolatedSpace::array_result_type& out_result, 
		const PolatedAxis in_depth, const PolatedAxis in_heel, const PolatedAxis in_trim ) 
	{
		ProError status;
		std::vector<double>::const_iterator heel_it;
		int heel_ix = 0;
		for (heel_it = in_heel.begin();  heel_it != in_heel.end(); ++heel_it, ++heel_ix) {

			switch( status = cf::create::Plane_Angle_adjust(m_total_selection, m_heel_pln_feature, *heel_it) ) {
			case PRO_TK_NO_ERROR: break;
			default:
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not adjust the heel " << status ;
				continue;
			}

			std::vector<double>::const_iterator trim_it;
			int trim_ix = 0;
			for (trim_it = in_trim.begin();  trim_it != in_trim.end(); ++trim_it, ++trim_ix) {

				switch( status = cf::create::Plane_Angle_adjust(m_total_selection, m_trim_pln_feature, *trim_it) ) {
				case PRO_TK_NO_ERROR: break;
				default:
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not adjust the trim " << status ;
					continue;
				}

				std::vector<double>::const_iterator depth_it;
				int depth_ix = 0;
				for (depth_it = in_depth.begin();  depth_it != in_depth.end(); ++depth_it, ++depth_ix)
				{
					isis_LOG(lg, isis_FILE, isis_INFO) << "parameters " << '\n'
						<< "  heel  [" << heel_ix << "] " << *heel_it << '\n'
						<< "  trim  [" << trim_ix <<  "] " << *trim_it << '\n'
						<< "  depth [" << depth_ix <<  "] " << *depth_it << '\n'
						;
						
					switch( status = cf::create::Plane_Offset_adjust(m_total_selection, m_depth_pln_feature, *depth_it) ) {
					case PRO_TK_NO_ERROR: break;
					default:
						isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not adjust the depth " << status ;
					}

					ProMassProperty work_mass_properties;
					wchar_t*  default_csys_name = NULL;
					switch( status = ProSolidMassPropertyGet(m_total_solid, default_csys_name, &work_mass_properties) ) {
					case PRO_TK_NO_ERROR: break;
					case PRO_TK_BAD_INPUTS:
						isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "the solid handle is invalid " ;
						return PRO_TK_GENERAL_ERROR; 
					case PRO_TK_E_NOT_FOUND: 
						isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "the specified coordinate system was not found. " ;
						return PRO_TK_GENERAL_ERROR; 
					case PRO_TK_GENERAL_ERROR: 
						isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "- A general error occurred and the function failed. " ;
						return PRO_TK_GENERAL_ERROR; 
					default:
						isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not acquire the properties of the wetted volume " << status ;
						return PRO_TK_GENERAL_ERROR; 
					} 

					// extract results for the target displacement:heel:trim values 
					Result result;
					{ // volume 
						result.volume = work_mass_properties.volume;
						isis_LOG(lg, isis_FILE, isis_INFO) << "volume: " << result.volume ;	
					}
					{ // target depth
						result.depth = *depth_it;
					}
					{ // center of buoyancy
						double* cob_pfc = work_mass_properties.center_of_gravity;

						result.cob(0) = cob_pfc[0];
						result.cob(1) = cob_pfc[1];
						result.cob(2) =  cob_pfc[2];
						isis_LOG(lg, isis_FILE, isis_INFO) << "cob: " << result.cob ;
					}
					{ // wetted area
						double area = work_mass_properties.surface_area;
						// subtract the cut surface area
					}
					{ // frontal projection of wetted area
					}
					out_result[depth_ix][heel_ix][trim_ix] = result;
				} // depth
			} // trim
		} // heel
		return PRO_TK_NO_ERROR; 
	}
};

/**
*/
ProError ExteriorShell::set_working_solid(ProSolid in_solid) {
	m_working_solid = in_solid;
	return PRO_TK_NO_ERROR; 
}

ProError ExteriorShell::set_wrapped_solid(ProSolid in_solid) {
	m_wrapped_solid = in_solid;
	return PRO_TK_NO_ERROR; 
}

ProError ExteriorShell::glom_working_solid() {
	ProError status;
	ProMdl model;
	switch( status = ProMdlCurrentGet(&model) ) {
	case PRO_TK_NO_ERROR: break;
	default:  
		isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "could not get-current-model" ;
		return PRO_TK_GENERAL_ERROR; 
	} 
	m_working_solid = static_cast<ProSolid>(model);
	return PRO_TK_NO_ERROR; 
}

ProError ExteriorShell::glom_wrapped_solid( ) {
	ProError status;
	ProMdl model;
	switch( status = ProMdlCurrentGet(&model) ) {
	case PRO_TK_NO_ERROR: break;
	default:  
		isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "could not get-current-model" ;
		return PRO_TK_GENERAL_ERROR; 
	} 
	m_wrapped_solid = static_cast<ProSolid>(model);
	return PRO_TK_NO_ERROR; 
}

ProError ExteriorShell::set_current_solid(ProSolid in_solid) {
	ProError status;
	switch( status = ProMdlDisplay(in_solid) ) {
	case PRO_TK_NO_ERROR: break;
	case PRO_TK_INVALID_PTR:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) 
			<< "the specified model is not in memory.";
		return PRO_TK_GENERAL_ERROR;
	case PRO_TK_E_NOT_FOUND:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) 
			<< "the <i>model</i> is NULL, there is no current object";
		return PRO_TK_GENERAL_ERROR;
	case PRO_TK_GENERAL_ERROR:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) 
			<< "there was a general error and the function failed.";
		return PRO_TK_GENERAL_ERROR;
	case PRO_TK_INVALID_TYPE:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) 
			<< "you specified an invalid model type.";
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) 
			<< "could not load model " << status;
		return PRO_TK_GENERAL_ERROR;
	}
	return PRO_TK_NO_ERROR;
}

ProError ExteriorShell::set_current_solid_to_working() {
	return set_current_solid(m_working_solid);
}

ProError ExteriorShell::set_current_solid_to_wrapped( ) {
	return set_current_solid(m_wrapped_solid);
}


/** 
activate a model.
Make the specified model current.
Current means it is the model registered in this external-shell object.

This does not make the model current (displayed) and activated
in the interactive Creo context.
*/
ProError ExteriorShell::activate_model( std::string in_name, ProMdlType in_model_type = PRO_MDL_PART ) {
	ProError status;
	ProFamilyName model_name;
	ProStringToWstring(model_name, const_cast<char*>(in_name.c_str())); 
	ProMdl model;

	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) 
		<< " name: " << model_name << " type: " << in_model_type ;
	switch( status = ProMdlRetrieve(model_name, in_model_type, &model) ) {
	case PRO_TK_NO_ERROR: break;
	case  PRO_TK_BAD_INPUTS:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "one or more of the input arguments are invalid. " 
			<< " name: " << model_name << " type: " << in_model_type ;
        return PRO_TK_GENERAL_ERROR;
	case PRO_TK_E_NOT_FOUND:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "no retrievable model was not found in the current directory." ;
        return PRO_TK_GENERAL_ERROR;
	case PRO_TK_NO_PERMISSION:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "retrieve does not have permission to operate on this model." ;
        return PRO_TK_GENERAL_ERROR;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not load model " << status << " " << in_name ;
		return PRO_TK_GENERAL_ERROR;
	}
	m_working_solid = static_cast<ProSolid>(model);
    return PRO_TK_NO_ERROR;
}


/**
Obtain an space of solutions over which interpolation may be performed.
*/
ProError ExteriorShell::computeHydrostatic( PolatedSpace::ptr& out_result ) const
{
	ProError rc = PRO_TK_NO_ERROR;
	if (m_wrapped_solid == NULL) {
		return PRO_TK_GENERAL_ERROR;
	}
	axis_type zero_axis;
	zero_axis.push_back(0.0);

	const PolatedAxis depth_axis(m_depth_axis.empty() ? zero_axis : m_depth_axis);
	const PolatedAxis trim_axis( m_trim_axis.empty() ? zero_axis : m_trim_axis);
	const PolatedAxis heel_axis( m_heel_axis.empty() ? zero_axis : m_heel_axis);

	long depth_size = depth_axis.size();
	long heel_size = heel_axis.size();
	long trim_size = trim_axis.size();
	
	Cutter cutter( m_wrapped_solid, m_long_orient, m_vert_orient );
	PolatedSpace::array_result_type result;
	// boost::extents[depth_size][heel_size][trim_size]); 

	cutter.computeHydrostatic(result, depth_axis, heel_axis, trim_axis);

	out_result = boost::make_shared<PolatedSpace>(depth_axis, heel_axis, trim_axis, result);
	return rc;
}

/**
Obtain a single result for a particular displacement:heel:trim combination.
In particular find the coresponding depth.
*/
ProError ExteriorShell::computeHydrostatic( Result& out_result, const bool retain,
	const double in_tolerance, const double in_displacement, 
	const double in_heel, const double in_trim ) const 
{
	if (m_wrapped_solid == NULL) {
		return PRO_TK_GENERAL_ERROR;
	}
	Cutter cutter( m_wrapped_solid, AXIS_Y_NEGATIVE, AXIS_Z_NEGATIVE );
	cutter.m_debug_retain_datums = retain;
	cutter.computeHydrostatic(out_result, in_tolerance, in_displacement, in_heel, in_trim );
	return PRO_TK_NO_ERROR;
}

	
} // namespace hydrostatic 
} // namespace isis 