#pragma once


#include "cc_LoggerBoost.h"


#include <set>
#include <map>
#include <boost/tuple/tuple.hpp>
#include <boost/multi_array.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/filesystem.hpp>

#include <ProSolid.h>
#include <ProToolkitErrors.h>

namespace isis {
namespace hydrostatic {

typedef ::boost::tuple<int, int, int, double> weighted_index_type;

typedef ::boost::numeric::ublas::vector<double> origin_type;
typedef ::boost::numeric::ublas::matrix<double> orientation_type;

// the below/above index and the ratio [0.0 1.0] 
typedef ::boost::tuple<int, int> index_range; 

/** types for Center-of-Gravity and Center-of-Hydrostatic */
typedef ::boost::numeric::ublas::vector<double> cog_type;
typedef ::boost::numeric::ublas::vector<double> cob_type;
typedef double area_type;
typedef double volume_type;
typedef double depth_type;
typedef double angle_type;

typedef struct { Pro3dPnt points[2]; } bounding_box_type;

/** the orientation of the water plane and a vector to a point on the surface. */
typedef ::boost::numeric::ublas::vector<double> vec_depth_type;

typedef ::std::vector<double> axis_type;  
typedef ::boost::multi_array<double, 3> array_type;

/** 
This class consists of a 3D array indexed by three vectors.jk
When "interpolate" is called with a pair of keys it finds the nearest 
four neighbors and performs a bilinear interpolation.
http://en.wikipedia.org/wiki/Bilinear_interpolation
If either of the keys lie outside the range of the indecies an
exception is thrown.
The keys can be checked with the 'inRange' method.
Extrapolation is also supported. 
*/
class PolatedAxis {
private:
	PolatedAxis(void)
		//: m_log_f(isis_FILE_CHANNEL),
		//  m_log_cf(isis_FILE_AND_CONSOLE_CHANNEL)
	{}
	axis_type m_axis;
public:
	typedef boost::shared_ptr<PolatedAxis> ptr;

	// the below/above index and the ratio [0.0 1.0] 
	typedef ::boost::tuple<index_range, double> inter_type;
	// the anchor/control index and the extension [0.0 inf] 
	typedef ::boost::tuple<index_range, double> extra_type;

	long size() const { return static_cast<long>(m_axis.size()); }

	bool isInRange(const double key) const; 
	inter_type interpolate(const double key) const; 
	extra_type extrapolate(const double key) const; 

	// virtual ~PolatedAxis(void); 
	PolatedAxis(const axis_type axis) 
		: //m_log_f(isis_FILE_CHANNEL),
		  //m_log_cf(isis_FILE_AND_CONSOLE_CHANNEL),
		  m_axis(axis) {}; 

public:
	//isisLogger  m_log_f;
	//isisLogger  m_log_cf;

public:
	std::vector<double>::const_iterator begin() const {
		return m_axis.begin();
	}
	std::vector<double>::const_iterator end() const {
		return m_axis.end();
	}
};

/**
A class of immutable Displaced-volume objects.
These objects are the result of processing an exterior-shell.
*/
class Result {
public:
	depth_type depth;
	volume_type total_volume;
	volume_type volume;
	cob_type cob;
	area_type wetted_area; 
	area_type projected_frontal_area; 
	std::vector< std::pair<double, double> > xsection_area;

	Result() : depth(0.0), volume(0.0), cob(3),
		wetted_area(0.0), projected_frontal_area(0.0),
		xsection_area(0) { }
};
std::ostream& operator<< (std::ostream& stream, const Result& result);


class PolatedSpace
{
public:
	typedef boost::shared_ptr<PolatedSpace> ptr;

	typedef ::boost::multi_array<Result, 3> array_result_type;

private:

	const PolatedAxis m_depth_axis;
	const PolatedAxis m_heel_axis;
	const PolatedAxis m_trim_axis;

	const array_result_type m_marray_result;

public:
	PolatedSpace(const PolatedAxis& in_depth, const PolatedAxis& in_heel, const PolatedAxis& in_trim, 
		const array_result_type& in_array_result )
		: //m_log_f(isis_FILE_CHANNEL),
		  //m_log_cf(isis_FILE_AND_CONSOLE_CHANNEL),
		  m_depth_axis(in_depth), m_heel_axis(in_heel), m_trim_axis(in_trim),
		  m_marray_result(in_array_result) {} 
	// virtual ~PolatedSpace(void);

	// hydrostatics methods
	::boost::tuple<bool,bool,bool> isInRange(const double in_depth, const double in_heel, const double in_trim);

	Result interpolate(const double in_depth, const double in_heel, const double in_trim);
	Result extrapolate(const double in_depth, const double in_heel, const double in_trim);

	std::ostream& print(std::ostream& inout_stream) const;
};
std::ostream& operator<< (std::ostream& stream, const PolatedSpace& hydrostatic);

/**
a set of values indicating the orientation of the shell in space.
*/
enum Orientation {
  AXIS_X_POSITIVE, AXIS_X_NEGATIVE,
  AXIS_Y_POSITIVE, AXIS_Y_NEGATIVE,
  AXIS_Z_POSITIVE, AXIS_Z_NEGATIVE,
};

/**
The exterior-shell is a closed envelope.  
It represents the potential hydraulic surface of a rigid body.
It is a factory which can produce "displaced-volume" objects.

The mass is not supplied, rather the displacement is supplied.
This shifts the computation of the object's mass and the
density of the displaced medium to the calling program.

The center-of-gravity is also left to the calling program.
The neutral position, righting moment and other related values
can be obtained with the information provided.
*/
class ExteriorShell
{
public:
	typedef boost::shared_ptr<ExteriorShell> ptr;
			
	ExteriorShell(const std::string name);
	void set_name(const std::string name) { m_name = name; }
	bool has_base_solid() { return (m_working_solid == NULL) ? false : true; }
	bool has_wrapped_solid() { return (m_wrapped_solid == NULL) ? false : true; }

	/** 
	The name is the name of the shell and can be anything.
	The path points to the creo part file which will be analyzed.
    */
	// virtual ~ExteriorShell(void);
	static ptr create(const std::string name ) {
        ptr shell_ptr(new isis::hydrostatic::ExteriorShell(name));
        return shell_ptr;
	}
	ProError set_working_solid(ProSolid in_solid);
	ProError set_wrapped_solid(ProSolid in_solid);

    ProError glom_working_solid();
    ProError glom_wrapped_solid();

	ProError set_current_solid(ProSolid in_solid);
	ProError set_current_solid_to_working();
	ProError set_current_solid_to_wrapped();

	ProError activate_model( std::string in_name, ProMdlType in_type );

    ProError create_shrinkwrap( std::string in_name );

	// ProError setOrigin( const origin_type in_origin );
	// ProError setOrientation( const orientation_type in_orient ); 

	ProError addDepth( const depth_type in_depth );
	ProError addHeelAngle( const angle_type in_heel_angle );
	ProError addTrimAngle( const angle_type in_trim_angle );

	ProError computeHydrostatic( PolatedSpace::ptr& out_result ) const;

	ProError computeHydrostatic( Result& out_result, const bool retain,
		const double in_tolerance,
		const double in_displacement, const double in_heel, const double in_trim ) const;	

	void debug_retain_datums( const bool value ){ m_retain_datums_flag = value; }

	void orientLongSet( const Orientation orient ) { m_long_orient = orient; }
	void orientVertSet( const Orientation orient ) { m_vert_orient = orient; }

private:

	/**
	Name of the exterior-shell.
	*/
	std::string m_name;

	/**
	Path to the Creo file which is the exterior-shell.
	*/
	::boost::filesystem::path m_path;
	::boost::filesystem::path m_new_path;

    /**
    The working solid
    */
    ProSolid m_working_solid;

    /**
    The shrinkwrapped solid
    */
    ProSolid m_wrapped_solid;

	/**
	the two axis indicating the orientation of the object in space.
	*/
	Orientation m_long_orient;
	Orientation m_vert_orient;

	/**
	The depth of the object.
	*/
	axis_type m_depth_axis;

	/**
	heel is the amount of rotation about the longitudinal axis.
	It is applied before trim.
	*/
	axis_type m_heel_axis;

	/**
	trim is the amount of rotation fore and aft.
	It is applied after heel.
	*/
	axis_type m_trim_axis;

	/**
	a debugging flag used to retain the constructed features
	used to perform the various actions.
	*/
	bool m_retain_datums_flag;
};

} // namespace hydrostatic
} // namespace isis

