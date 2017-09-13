
#define _USE_MATH_DEFINES
#include <math.h>

#include <JointCreo.h>

#include <ProFeatType.h>
#include <ProAxis.h>
#include <ProEdge.h>
#include <ProSurface.h>
#include <ProCsys.h>
#include <ProPoint.h>
#include <ProFeature.h>
#include <ostream>
#include <isis_application_exception.h>
#include "cc_LoggerBoost.h"

namespace isis {
namespace cad {
namespace creo {

// the number of turns
static double TAU = 2.0 * M_PI;

struct Marker {
public:
	//isisLogger  log_cf;

	// absolute location in the assembly coordinate system.
	e3ga::vector locate;
	e3ga::vector orient;
	JointType joint_type;
	// in pieces of pi [-1.0 to 1.0]
	double rotate;

	Marker()
		: //log_cf(isis_FILE_AND_CONSOLE_CHANNEL),
		locate(), orient(), rotate(0.0)
	{
	}
};
std::ostream& operator<<(std::ostream& out, const Marker mark) {
	out << " locate = [" << mark.locate.toString() << "]";
	out << " orient = [" << mark.orient.toString() << "]";
	out << " rotate = (" << mark.rotate << ")";
	return out;
}

e3ga::vector convert(ProVector& vec) {
	return e3ga::vector(e3ga::vector::coord_e1_e2_e3,
		vec[0], vec[1], vec[2] );
}

/**
return true if the value is in the tolerance range
near 0.0 and tau.
*/
bool within_angular_tolerance( const double& angle ) {
	// force normalized angle to be in the range [0.0 tau)
	
	double normal_angle = fmod(abs(angle), TAU);

	if (TAU < angle + isis::cad::DEFAULT_TOLERANCE) return true;
	if (angle < DEFAULT_TOLERANCE) return true;
	return false;
}

/**
 object: The pointer to the object being visited 
 status: The status returned by filter function
 app_data: In fact it's ProArray** of ProGeomitem.
*/
ProError extract_point_marker( ProGeomitem& in_geom_item, Marker& out_marker )
{
	ProError rc = PRO_TK_NO_ERROR;
	//log4cpp::CategoryStream& log = out_marker.log_cf.infoStream();

	ProPoint point;
	switch( rc = ProGeomitemToPoint(&in_geom_item, &point) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << " geom to point error";
	}

	ProGeomitemdata* geom_item_data = NULL;

	ProVector location;
	switch( rc = ProPointCoordGet(point, location) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "point coord error";
	}
	out_marker.locate = convert(location);
	out_marker.joint_type = DATUM_POINT;

	// log << "marker point";
	return rc;
}

ProError get_point_marker(
	ProGeomitem* geom_item, ProError  status, ProAppData  app_data)
{
	ProError rc = PRO_TK_NO_ERROR;
	Marker& marker = *reinterpret_cast<Marker*>(app_data);
    return extract_point_marker( *geom_item, marker );
}

ProError extract_axis_marker( ProGeomitem& in_geom_item, Marker& out_marker )
{
	ProError rc = PRO_TK_NO_ERROR;
	//log4cpp::CategoryStream& log = out_marker.log_cf.infoStream();

	ProAxis axis;
	switch( rc = ProGeomitemToAxis(&in_geom_item, &axis) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "geom to axis error";
	}

	ProGeomitemdata* geom_item_data = NULL;
	switch( rc = ProAxisDataGet(axis, &geom_item_data) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "axis data get error";
	}
	ProCurvedata& axis_data = *geom_item_data->data.p_curve_data;
	ProEnttype geom_type;
	Pro3dPnt p1, p2;
	rc = ProCurvedataTypeGet(&axis_data, &geom_type);
	switch(geom_type) {
	case PRO_ENT_LINE:
		rc = ProLinedataGet(&axis_data, p1, p2);
		break;
	default:
		ProGeomitemdataFree( &geom_item_data );
		return PRO_TK_CONTINUE;
	}
	ProGeomitemdataFree( &geom_item_data );

	out_marker.locate.set_e1( p1[0] );
	out_marker.orient.set_e1( p2[0] - p1[0] );

	out_marker.locate.set_e2( p1[1] );
	out_marker.orient.set_e2( p2[1] - p1[1] );

	out_marker.locate.set_e3( p1[2] );
	out_marker.orient.set_e3( p2[2] - p1[2] );

	out_marker.joint_type = DATUM_AXIS;

	return rc;
}

ProError get_axis_marker(
	ProGeomitem* geom_item, ProError  status, ProAppData  app_data)
{
	ProError rc = PRO_TK_NO_ERROR;
	Marker& marker = *reinterpret_cast<Marker*>(app_data);

    return extract_axis_marker( *geom_item, marker );
}

ProError extract_edge_marker( ProGeomitem& in_geom_item, Marker& out_marker )
{
	ProError rc = PRO_TK_NO_ERROR;
	//log4cpp::CategoryStream& log = out_marker.log_cf.infoStream();

	ProEdge edge;
	switch( rc = ProGeomitemToEdge(&in_geom_item, &edge) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "geom to axis error";
	}

	ProGeomitemdata* geom_item_data = NULL;
	switch( rc = ProEdgeDataGet(edge, &geom_item_data) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "axis data get error";
	}
	ProCurvedata& edge_data = *geom_item_data->data.p_curve_data;
	ProEnttype geom_type;
	Pro3dPnt p1, p2;
	rc = ProCurvedataTypeGet(&edge_data, &geom_type);
	switch(geom_type) {
	case PRO_ENT_LINE:
		rc = ProLinedataGet(&edge_data, p1, p2);
		break;
	default:
		ProGeomitemdataFree( &geom_item_data );
		return PRO_TK_CONTINUE;
	}
	ProGeomitemdataFree( &geom_item_data );

	out_marker.locate.set_e1( p1[0] );
	out_marker.orient.set_e1( p2[0] - p1[0] );

	out_marker.locate.set_e2( p1[1] );
	out_marker.orient.set_e2( p2[1] - p1[1] );

	out_marker.locate.set_e3( p1[2] );
	out_marker.orient.set_e3( p2[2] - p1[2] );

	out_marker.joint_type = DATUM_AXIS;


	return rc;
}

ProError extract_plane_marker( ProGeomitem* in_geom_item, Marker& out_marker )
{
	ProError rc = PRO_TK_NO_ERROR;
	//log4cpp::CategoryStream& log = out_marker.log_cf.infoStream();

	ProSurface plane;
	switch( rc = ProGeomitemToSurface(in_geom_item, &plane) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "geom to surface error";
	}

	ProGeomitemdata* geom_item_data = NULL;
	switch( rc = ProSurfaceDataGet(plane, &geom_item_data) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "surface data get error";
	}
	ProSurfacedata* surface_data = geom_item_data->data.p_surface_data;

	ProSrftype surface_type;
	ProUvParam uv_min, uv_max;
	ProSurfaceOrient surface_orient; 
	ProSurfaceshapedata surf_shape_data;
	int surface_id;
	rc = ProSurfacedataGet(surface_data, &surface_type, 
		uv_min, uv_max, &surface_orient, &surf_shape_data, 
		&surface_id);

	ProVector e_x, e_y, e_z;
	Pro3dPnt origin;
	rc = ProPlanedataGet( &surf_shape_data, e_x, e_y, e_z, origin);
	
	switch (surface_type)
		{
			case PRO_SRF_PLANE:
				// Good case
				rc = ProPlanedataGet( &surf_shape_data, e_x, e_y, e_z, origin);
				out_marker.joint_type = DATUM_PLANE;
				break;
			case PRO_SRF_CYL:
				double radius; // Not used
				rc = ProCylinderdataGet( &surf_shape_data, e_x, e_y, e_z, origin, &radius);
				out_marker.joint_type = DATUM_AXIS;
				break;
			default:
				// Not-supported yet
				isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "surface type is wrong: " << surface_type;
				return PRO_TK_BAD_INPUTS;
	}

	ProGeomitemdataFree( &geom_item_data );

	out_marker.locate = convert( origin );
	out_marker.orient = convert( e_z );
	return rc;
}

ProError get_plane_marker(
	ProGeomitem* geom_item, ProError  status, ProAppData  app_data)
{
	ProError rc = PRO_TK_NO_ERROR;
	Marker& marker = *reinterpret_cast<Marker*>(app_data);
	//log4cpp::CategoryStream& log = marker.log_cf.infoStream();
	return extract_plane_marker( geom_item, marker);
}


/**
The CSYS marker is directed along the (1 1 1) axis with a rotation.
http://en.wikipedia.org/wiki/Rotation_formalisms_in_three_dimensions#Rotation_matrix_.E2.86.94_Euler_axis.2Fangle
*/
ProError extract_csys_marker( ProGeomitem& in_geom_item, Marker& out_marker )
{
	//log4cpp::CategoryStream& log = out_marker.log_cf.infoStream();
	ProError rc = PRO_TK_NO_ERROR;

	ProCsys csys;
	rc = ProGeomitemToCsys(&in_geom_item, &csys);

	ProGeomitemdata* geom_item_data = NULL;
	switch( rc = ProCsysDataGet(csys, &geom_item_data) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "csys data get error";
	}
	ProCsysdata* csys_data = geom_item_data->data.p_csys_data;

	out_marker.locate = convert( csys_data->origin );

	e3ga::vector basis_x = convert( csys_data->x_vector );
	e3ga::vector basis_y = convert( csys_data->y_vector );
	e3ga::vector basis_z = convert( csys_data->z_vector );

	double rotate = acos(0.5 * 
		(basis_x.get_e1() + basis_y.get_e2() + basis_z.get_e3() - 1.0));
	out_marker.rotate = rotate / TAU;

	if ( within_angular_tolerance( rotate )) {
		out_marker.orient = 
			e3ga::vector( e3ga::vector::coord_e1_e2_e3,
			1.0, 0.0, 0.0 );
		return rc;
	} 
	out_marker.orient = 0.5 / sin( rotate ) *
		e3ga::vector( e3ga::vector::coord_e1_e2_e3,
		basis_z.get_e2() - basis_y.get_e3(),
		basis_x.get_e3() - basis_z.get_e1(),
		basis_y.get_e1() - basis_x.get_e2() );
	
	out_marker.joint_type = DATUM_CSYS;

	// log << "marker csys";
	return rc;
}

ProError get_csys_marker(
	ProGeomitem* geom_item, ProError  status, ProAppData  app_data)
{
	Marker& marker = *reinterpret_cast<Marker*>(app_data);
    return extract_csys_marker( *geom_item, marker );
}



/**
Extract the information from a set of Creo datums to produce
a set of datum joints.
*/
Joint convert_datum(ProFeature& in_datum) {
	ProError rc;
	ProFeattype feat_type;
	Marker marker;
	//log4cpp::CategoryStream& log = marker.log_cf.infoStream();

	switch( rc = ProFeatureTypeGet(&in_datum, &feat_type) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "feature type get error";
	}
	switch( feat_type ) {
	case PRO_FEAT_DATUM_POINT:
		// R.O. Was 4/30/2015 switch( rc = ProFeatureGeomitemVisit(&in_datum, PRO_AXIS, 
		switch( rc = ProFeatureGeomitemVisit(&in_datum, PRO_POINT, 
			reinterpret_cast<ProGeomitemAction>(get_point_marker),
			NULL, static_cast<ProAppData>(&marker) ) ) 
		{
		case PRO_TK_NO_ERROR: break;
		default:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "point feature geom visit error";
		}
		break;
	case PRO_FEAT_DATUM_AXIS:
		switch( rc = ProFeatureGeomitemVisit(&in_datum, PRO_AXIS, 
			reinterpret_cast<ProGeomitemAction>(get_axis_marker),
			NULL, static_cast<ProAppData>(&marker) ) ) 
		{
		case PRO_TK_NO_ERROR: break;
		default:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "axis feature geom visit error";
		}
		break;
	case PRO_FEAT_DATUM:
	case PRO_FEAT_DATUM_SURF:
		switch( rc = ProFeatureGeomitemVisit(&in_datum, PRO_SURFACE, 
			reinterpret_cast<ProGeomitemAction>(get_plane_marker),
			NULL, static_cast<ProAppData>(&marker) ) ) 
		{
		case PRO_TK_NO_ERROR: break;
		default:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "surface feature geom visit error";
		}
		break;
	case PRO_FEAT_CSYS:
		switch( rc = ProFeatureGeomitemVisit(&in_datum, PRO_CSYS, 
			reinterpret_cast<ProGeomitemAction>(get_csys_marker),
			NULL, static_cast<ProAppData>(&marker) ) ) 
		{
		case PRO_TK_NO_ERROR: break;
		default:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "csys feature geom visit error";
		}
		break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "unknown feature type error";
	}

	Joint joint(marker.joint_type, 
		marker.locate, marker.orient, marker.rotate, 0);
	return joint;
}


std::vector<Joint> convert_datum_set( std::vector<ProFeature> in_datums ) {
	std::vector<Joint> joints(in_datums.size());
	for each (ProFeature feat in in_datums) {
		joints.push_back( convert_datum( feat ) );
	}
	return joints;
}


/**
Extract the information from a set of Creo datums to produce
a set of datum joints.
*/
Joint extract_marker( ProGeomitem& in_geom_item) {
	Marker marker;
	//log4cpp::CategoryStream& log = marker.log_cf.infoStream();

	switch( in_geom_item.type ) {
	case PRO_SURFACE:
		extract_plane_marker( &in_geom_item, marker );
		//extract_point_marker( in_geom_item, marker );
		break;
	case PRO_AXIS:
		extract_axis_marker( in_geom_item, marker );
		break;
	case PRO_EDGE:
		extract_edge_marker( in_geom_item, marker );
		break;
	case PRO_POINT:
		extract_point_marker( in_geom_item, marker );
		break;
	case PRO_CSYS:
		extract_csys_marker( in_geom_item, marker );
		break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "unknown feature type error";
	}

	Joint joint(marker.joint_type, 
		marker.locate, marker.orient, marker.rotate, 0);
	return joint;
}

Joint extract_marker( ProSelection& in_selection) {
	ProError rc;
	ProModelitem model_item;

	switch( rc = ProSelectionModelitemGet( in_selection, &model_item ) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		std::stringstream errorString;
		errorString 
			<< "could not get the model item for the selection.";
		throw isis::application_exception(errorString.str());
	}
	switch( model_item.type ) {
	case PRO_FEATURE:
		return convert_datum( static_cast<ProFeature>(model_item) );
	}
	return extract_marker( static_cast<ProGeomitem>(model_item) );
}

/**
 Do not use this function as it ties CAD specific 
 types to tightly with the agnostic joint.

Joint infer_joint(std::vector<ProFeature> datum_set) {
	return infer_joint(convert_datum_set(datum_set));
}
*/

} // creo
} // cad
} // isis
