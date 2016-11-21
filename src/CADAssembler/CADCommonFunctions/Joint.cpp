
#define _USE_MATH_DEFINES
#include <cmath>

#include "Joint.h"
#include "GraphicsFunctions.h"

#include "c3ga.h"

namespace isis {
namespace cad {

const double TAU = 2.0 * M_PI;
const double Joint::isis_PI = M_PI;

std::map<JointType, std::string> joint_type_code_map
	= boost::assign::map_list_of
	(fff,"fff")(xff, "xff")(iff,"iff")(off,"off")
	(xxf,"xxf")(xif,"xif")(oxf,"oxf")(iif,"iif")
	(oif,"oif")(oof,"oof")(xxx,"xxx")(xxi,"xxi")
	(oxx,"oxx")(xii,"xii")(oxi,"oxi")(oox,"oox")
	(iii,"iii")(oii,"oii")(ooi,"ooi")(ooo,"ooo") ;

std::map<JointType, std::string> joint_type_name_map
	= boost::assign::map_list_of
	(COMPOSITE,"composite")
	(ooo,"fixed")(ooi,"revolute")
	(oii,"universal")(iii,"spherical")
	(oox,"prismatic")(xxi,"planar")
	(oof,"cylindrical")(fff,"free") ;

std::map<JointType, std::string> joint_type_datum_map
	= boost::assign::map_list_of
	(ooo,"csys") (xxi,"plane") (oof,"line") (iii,"point") ;

joint_type_map_t joint_type_map
	= boost::assign::map_list_of
	("fff",fff)("xff",xff)("iff",iff)("off",off)
	("xxf",xxf)("xif",xif)("oxf",oxf)("iif",iif)
	("oif",oif)("oof",oof)("xxx",xxx)("xxi",xxi)
	("oxx",oxx)("xii",xii)("oxi",oxi)("oox",oox)
	("iii",iii)("oii",oii)("ooi",ooi)("ooo",ooo)
	("fixed",ooo)("csys",ooo)
	("revolute",ooi)("pin",ooi)("hinge",ooi)
	("universal",oii)
	("spherical",iii)("ball",iii)("point",iii)
	("prismatic",oox)("slider",oox)
	("planar",xxi)("plane",xxi)
	("cylindrical",oof)("line",oof)("axis",oof)
	("free",fff) ;

std::ostream& operator<<( std::ostream& out, JointType& that ) {
	// out << " [ " << static_cast<int>(that) << " ] ";
	if (joint_type_name_map.count(that) == 1) {
		out << joint_type_name_map[that];
	} else 
	if (joint_type_code_map.count(that) == 1) {
		out << "<" << joint_type_code_map[that] << ">";
	} else {
		out << "[" << static_cast<int>(that) << "]";
	}

	// if (joint_type_datum_map.count(that) == 1) {
	// 	out << " : datum-" << joint_type_datum_map[that];
	// }
	return out;
}

/*
log4cpp::CategoryStream& operator<<(log4cpp::CategoryStream& out, const JointType& that) {
	if (joint_type_name_map.count(that) == 1) {
		out << joint_type_name_map[that];
	} else 
	if (joint_type_code_map.count(that) == 1) {
		out << "<" << joint_type_code_map[that] << ">";
	} else {
		out << "[" << static_cast<int>(that) << "]";
	}
	return out;
}
*/

/*  zzz fix this
isis::isisLogger& operator<<(isis::isisLogger& out, const JointType& that) {
	if (joint_type_name_map.count(that) == 1) {
		out << joint_type_name_map[that];
	} else 
	if (joint_type_code_map.count(that) == 1) {
		out << "<" << joint_type_code_map[that] << ">";
	} else {
		out << "[" << static_cast<int>(that) << "]";
	}
	return out;
}
*/

std::ostream& operator<<(std::ostream& out, e3ga::vector& that) {
	out << " [ " << that.c_str() << " ] ";
	return out;
}
/*
log4cpp::CategoryStream& operator<<(log4cpp::CategoryStream& out, const e3ga::vector& that) {
	out << " [ " << that.c_str() << " ] ";
	return out;
}
*/

/* R.O. Not using this anymore.  Replaced with the equivalent code of " [ " << that.c_str() << " ] "
isis::isisLogger& operator<<(isis::isisLogger& out, const e3ga::vector& that) {
	out << " [ " << that.c_str() << " ] ";
	return out;
}
*/


std::ostream& operator<<( std::ostream& out, Joint& that ) {
	out << " locate = [" << that.location.toString() << "] "
		<< " orient = [" << that.orientation.toString() << "] "
		<< " rotate = " << that.rotation / TAU << " turn "
		<< " type = " << static_cast<JointType>(that.type);
	return out;
}

/*
log4cpp::CategoryStream& operator<<(log4cpp::CategoryStream& out, const Joint& that) {
	out << " locate = [" << that.location.toString() << "] "
		<< " orient = [" << that.orientation.toString() << "] "
		<< " rotate = " << that.rotation / TAU << " turn "
		<< " type = " << static_cast<JointType>(that.type);
	return out;
}
*/

/* use that.toString operator that calls std::ostream& operator<<( std::ostream& out, Joint& that )
isis::isisLogger& operator<<(isis::isisLogger& out, const Joint& that) {
	out << " locate = [" << that.location.toString() << "] "
		<< " orient = [" << that.orientation.toString() << "] "
		<< " rotate = " << that.rotation / TAU << " turn "
		<< " type = " << static_cast<JointType>(that.type);
	return out;
}
*/

std::string Joint::toString() {
	std::ostringstream ss;
	ss << *this;
	return ss.str();
}

/**
 get the location.
*/
e3ga::vector Joint::getLocationAbsolute() {
	return this->location;
}

/**
 Get the orientation as a set of 313 euler angles.
 see http://en.wikipedia.org/wiki/Rotation_formalisms_in_three_dimensions#Euler_axis.2Fangle_.E2.86.94_quaternion
*/
e3ga::vector Joint::getOrientation313Absolute() {
	e3ga::vector q = e3ga::unit(orientation);
	// e3ga::vector origin;
	// bg::assign_zero(origin);
	double rot_rad = rotation * M_PI;
	double qr = cos( rot_rad );
	double sin_rad = sin( rot_rad / 2.0 );

	double qi = q.get_e1();
	double qj = q.get_e2();
	double qk = q.get_e3();

	e3ga::vector euler;
	euler.set_e1( atan2( (qr*qj + qi*qk), (qr*qi - qj*qk) ) );
	euler.set_e1( acos( qr*qr - qi*qi - qj*qj + qk*qk ) );
	euler.set_e1( atan2( (qr*qj + qi*qk), (qr*qi + qj*qk) ) );
	return euler;
}

/**
If you want to force the object to change its state.
This is useful for placing joints in a collection.
*/
Joint& Joint::operator=( const Joint& that ) {
	JointType* re_type = const_cast<JointType*>(&type);
	*re_type = that.type;

	e3ga::vector* re_locate = const_cast<e3ga::vector*>(&location);
	*re_locate = that.location;

	e3ga::vector* re_orient = const_cast<e3ga::vector*>(&orientation);
	*re_orient = that.orientation;

	double* re_rotate = const_cast<double*>(&rotation);
	*re_rotate = that.rotation;

	return *this;
}


c3ga::normalizedPoint c3ga_point( const e3ga::vector pnt ) {
	return c3ga::cgaPoint( pnt.get_e1(), pnt.get_e2(), pnt.get_e3() );
}

/**
Convert a joint marker into a c3ga point.
*/
c3ga::normalizedPoint c3gaPoint( const Joint joint ) {
	return c3ga_point( joint.location );
}

/**
Convert a joint marker into a c3ga line.
*/
c3ga::line c3gaLine( const Joint joint ) {
	c3ga::normalizedPoint locate_pnt = c3ga_point( joint.location );
	c3ga::normalizedPoint orient_vec = c3ga_point( joint.orientation );
	return c3ga::line
		(locate_pnt ^ (locate_pnt + orient_vec) ^ c3ga::ni, 0);
}

/**
Convert a joint marker into a c3ga plane.
*/
c3ga::plane c3gaPlane( const Joint joint ) {
	c3ga::normalizedPoint locate_pnt = c3ga_point( joint.location );
	c3ga::normalizedPoint orient_vec = c3ga_point( joint.orientation );
	return c3ga::plane
		(locate_pnt ^ dual(orient_vec), 0);
}



/**
Adding constraints can only reduce the degrees of freedom.
*/

/**
Fuse two planes (xxi):
<ul>
<li> planar : if the planes are parallel.
<li> prismatic : if the planes are orthogonal.
</ul>
*/
Joint meld_xxi_xxi(const Joint& major, const Joint& minor) {
	//log4cpp::CategoryStream& log = major.log_cf.infoStream();
	//log4cpp::CategoryStream& log_debug = major.log_cf.debugStream();
	//log << "meld_xxi_xxi";

	// 

	isis_LOG(lg, isis_FILE, isis_INFO) << "meld_xxi_xxi";

	e3ga::vector b1 = major.orientation;
	e3ga::vector b2 = minor.orientation;

	// compute the "meet" of the two planes 

	if ( e3ga::zero( b2 ^ b1, DEFAULT_TOLERANCE ) ) {
		// parallel planes
		return major;
	}

	e3ga::vector orient = b2 << *b1;
	orient = e3ga::unit(orient);
	/**
	 The prismatic orientation is computed 
	 from the scalar-product of the two planes.

	 The prismatic location is arbitrary.
	 The convention is to select the point on the
	 orientation closest to the location of the major plane.
	 The arbitrariness of the location point is indicated.
	*/
	e3ga::vector x1 = major.location;
	e3ga::vector x2 = minor.location;

	double p1 = b1 % x1;
	double p2 = b2 % x2;

	isis_LOG(lg, isis_FILE, isis_DEBUG) << "hessians = " << isis_EOL
		<< "-<p>" << " = " << "<n>" << " . " << "<x>" << isis_EOL
		<< "-" << p1 << " = " << " [ " << b1.c_str() << " ] " << " . " << " [ " <<  x1.c_str() << " ] " << isis_EOL
		<< "-" << p2 << " = " << " [ " << b2.c_str() << " ] " << " . " << " [ " <<  x2.c_str() << " ] ";
	
	double p3 = orient % x1;
	e3ga::vector pd(e3ga::vector::coord_e1_e2_e3, p1,p2,p3);

	// apply cramer's rule
	double denom = norm(b1 ^ b2 ^ orient);
	e3ga::vector locate(e3ga::vector::coord_e1_e2_e3, 
		norm(p3 ^ b2 ^ orient)/denom,
		norm(b1 ^ p3 ^ orient)/denom,
		norm(b1 ^ b2 ^ p3)/denom);

	Joint result(PRISMATIC, locate, orient, 0.0, major, minor);
	isis_LOG(lg, isis_FILE, isis_INFO) << result.toString();
	return result;
}

/**
Merging a plane (xxi) and spherical (iii) constraint resulting in:
<ul>
<li> a pin joint : all cases
</ul>
http://mathworld.wolfram.com/Point-PlaneDistance.html
*/
Joint meld_xxi_iii(const Joint& plane, const Joint& ball, const bool flip = false) {
	//log4cpp::CategoryStream& log = plane.log_cf.infoStream();

	// 
	
	isis_LOG(lg, isis_FILE, isis_INFO) << (flip ? "meld_iii_xxi" : "meld_xxi_iii");


	e3ga::vector x1 = plane.location;
	e3ga::vector x2 = ball.location;
	e3ga::vector b1 = e3ga::unit(plane.orientation);

	/**
	b1 : normal to the plane
	d1 : distance from plane to point
	x2 : point location
	x1 : plane location
	r1 : location of the revolute, projection of the point onto the plane.

		r1 + (b1 * d1) = x2;
	&
		d1 = (x2 - x1) dot b1;
	=>
		r1 = x2 - [(x2 - x1) dot b1] * b1;
	*/
	e3ga::vector r1 = x2 - (b1 << ((x2 - x1) % b1));

	Joint result( REVOLUTE, r1, b1, 0.0, plane, ball );
	return result;
}
Joint meld_iii_xxi(const Joint& ball, const Joint& plane) {
	return meld_xxi_iii(plane, ball, true);
}

/**
merging a revolute (ooi) and a plane (xxi) constraint may result in:
<ul>
<li> a revolute : plane perpendicular to the axis
<li> fixed : all other cases
</ul>
*/
Joint meld_ooi_xxi(const Joint& pin, const Joint& plane, const bool flip = false) {
	//log4cpp::CategoryStream& log = pin.log_cf.infoStream();

	// 
	isis_LOG(lg, isis_FILE, isis_INFO) << (flip ? "meld_xxi_ooi" : "meld_ooi_xxi");

	e3ga::vector x1 = plane.location;
	e3ga::vector x2 = pin.location;

	e3ga::vector b1 = e3ga::unit(plane.orientation);
	e3ga::vector b2 = e3ga::unit(pin.orientation);

	e3ga::vector orient = b2 << *b1;
	if ( e3ga::zero( orient, DEFAULT_TOLERANCE ) ) {
		// plane perpendicular to revolute axis
		return pin;
	}
	Joint result(FIXED, x1, b1, 0.0, pin, plane);
	return result;
}
Joint meld_xxi_ooi(const Joint& plane, const Joint& pin) {
	return meld_ooi_xxi(pin, plane, true);
}

/**
Merging a plane (xxi) and an axix (oof) constraint may result in:
<ul>
<li> prismatic : cylindar's axis lying in the plane
<li> revolute : plane perpendicular to the cylindar's axis
<li> fixed : all other cases
</ul>
*/
Joint meld_xxi_oof(const Joint& plane, const Joint& axis, const bool flip = false) {
	//log4cpp::CategoryStream& log = plane.log_cf.infoStream();

	// 
	
	isis_LOG(lg, isis_FILE, isis_INFO) << (flip ? "meld_oof_xxi" : "meld_xxi_oof");

	e3ga::vector x1 = axis.location;

	e3ga::vector b3 = e3ga::unit(axis.orientation);
	e3ga::vector b2 = e3ga::unit(plane.orientation);

	if ( e3ga::zero((b2 ^ b3), DEFAULT_TOLERANCE) ) {
		// axis perpendicular to plane = revolute 
		/**
		Find the intersection between the axis and the plane.
		http://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
		*/
		e3ga::vector x2 = plane.location;

		double d = ((x2 - x1) % b2) / (b3 % b2);
		e3ga::vector r3 = (d * b3) + x1;
		
		Joint result(REVOLUTE, r3, b3, 0.0, plane, axis);
		return result;
	}

	/**
http://en.wikipedia.org/wiki/Rotation_formalisms_in_three_dimensions#Rotation_matrix_.E2.86.94_Euler_axis.2Fangle
	*/
	e3ga::vector b1 = e3ga::unit( b2 << *b3 );
	double theta = acos(0.5*
		(b1.get_e1() + b2.get_e2() + b3.get_e3() - 1.0));

	if ( e3ga::zero( (b2 % b3) , DEFAULT_TOLERANCE) ) {
		// axis parallel to plane = prismatic
		// is the axis or the plane the reference location?
		// if (flip) 
		//  b? is the projection of the axis onto the plane.
		// 	return Joint(PRISMATIC, x1, b?, -theta, plane, axis);
		return Joint(PRISMATIC, x1, b3, theta, plane, axis);
	}
	double half_csc_theta = 0.5 / sin( theta );

	// compute the euler axis
	e3ga::vector ea(e3ga::vector::coord_e1_e2_e3,
		half_csc_theta * (b3.get_e2() - b2.get_e3()),
		half_csc_theta * (b1.get_e3() - b3.get_e1()),
		half_csc_theta * (b2.get_e1() - b1.get_e2()) );

	// compute the location for the fixed joint
	// 1 - intersection between the axis and the plane.
	// 2 - the location of the axis.
	// 3 - the location of the plane.
	// 
	// 1 is probably the best choice but 2 is easier.
	e3ga::vector x2 = axis.location;  
	return Joint(FIXED, x2, ea, theta, plane, axis);
}

Joint meld_oof_xxi(const Joint& axis, const Joint& plane) {
	return meld_xxi_oof(plane, axis, true);
}

/**
merging a pair of revolute (ooi) joints may result in:
<ul>
<li> fixed : axies not parallel
<li> revolute : coincident orientations axis
</ul>
*/
Joint meld_ooi_ooi( const Joint& major, const Joint& minor) {
	//log4cpp::CategoryStream& log = major.log_cf.infoStream();
	//log << "meld_ooi_ooi";
	//major.log_cf.infoStream() << "meld_ooi_ooi" ;

	// 
	isis_LOG(lg, isis_FILE, isis_INFO) << "meld_ooi_ooi";

	// Are the axies parallel?
	e3ga::vector b1 = e3ga::unit(major.orientation);
	e3ga::vector b2 = e3ga::unit(minor.orientation);

	e3ga::bivector b1b2 = b1 ^ b2;
	if ( ! e3ga::zero( b1b2, DEFAULT_TOLERANCE ) ) {
		// the axies are *NOT* parallel
		Joint result(FIXED, 
			major.location, major.orientation, major.rotation,
			major, minor);
		return result;
	}

	// Are the axis colinear?
	e3ga::vector x1 = major.location;
	e3ga::vector x2 = minor.location;
	e3ga::bivector x1x2b1 = (x2 - x1) ^ b1;
	if ( e3ga::zero(x1x2b1, DEFAULT_TOLERANCE) ) {
		// the axis are colinear
		return major;
	}
	return minor;
}

/**
merging a pair of cylinderical (oof) joints may result in:
<ul>
<li> fixed : axies not parallel
<li> cylindrical : coincident orientations axis
<li> prismatic : parallel (but not coincident) orientations axis
</ul>
*/
Joint meld_oof_oof(const Joint& major, const Joint& minor) {
	//log4cpp::CategoryStream& log = major.log_cf.infoStream();
	//log << "meld_oof_oof";

	// 
	isis_LOG(lg, isis_FILE, isis_INFO) << "meld_oof_oof";

	// Are the axies parallel?
	e3ga::vector b1 = e3ga::unit(major.orientation);
	e3ga::vector b2 = e3ga::unit(minor.orientation);

	e3ga::bivector b1b2 = b1 ^ b2;
	if ( ! e3ga::zero( b1b2, DEFAULT_TOLERANCE ) ) {
		// the axies are *NOT* parallel
		Joint result(FIXED, 
			major.location, major.orientation, major.rotation,
			major, minor);
		return result;
	}

	// Are the axis colinear?
	e3ga::vector x1 = major.location;
	e3ga::vector x2 = minor.location;
	e3ga::bivector x1x2b1 = (x2 - x1) ^ b1;
	if ( e3ga::zero(x1x2b1, DEFAULT_TOLERANCE) ) {
		// the axis are colinear
		return major;
	}

	// the axies are parallel and offset.
	// http://en.wikipedia.org/wiki/Rotation_formalisms_in_three_dimensions#Rotation_matrix_.E2.86.94_Euler_axis.2Fangle
	//
	e3ga::vector& a1 = b1;
	e3ga::vector a2 = e3ga::unit(*x1x2b1);
	e3ga::vector a3 = *(a1 ^ a2);
	double theta = acos(0.5*
		(b1.get_e1() + a2.get_e2() + a3.get_e3() - 1.0));

	Joint result(PRISMATIC, x1, b1, theta / M_PI,
		major, minor);
	return result;
}

/**
merging a revolute (ooi) and a universal (oii) joint:
<ul>
</ul>
*/
Joint meld_ooi_oii(const Joint& pin, const Joint& universal, const bool flip = false) {
	//log4cpp::CategoryStream& log = pin.log_cf.infoStream();
	//log << (flip ? "meld_oii_ooi" : "meld_ooi_oii");

	// 
	isis_LOG(lg, isis_FILE, isis_INFO) << (flip ? "meld_oii_ooi" : "meld_ooi_oii");

	return pin.make_composite(universal);
}
Joint meld_oii_ooi(const Joint& universal, const Joint& pin) {
	return meld_ooi_oii(pin, universal, true);
}

/**
merging a pair of prismatic (oox) joints may result in:
<ul>
<li> prismatic : parallel orientations axis
<li> fixed : all other cases
</ul>
*/
Joint meld_oox_oox(const Joint& major, const Joint& minor) {
	//log4cpp::CategoryStream& log = major.log_cf.infoStream();
	//log << "meld_oox_oox";

	// 
	isis_LOG(lg, isis_FILE, isis_INFO) <<  "meld_oox_oox";

	e3ga::vector b1 = e3ga::unit(major.orientation);
	e3ga::vector b2 = e3ga::unit(minor.orientation);
	e3ga::bivector b1b2 = b1 ^ b2;
	if ( e3ga::zero( b1b2, DEFAULT_TOLERANCE ) ) {
		return major;
	}
	Joint result(FIXED, 
		major.location, major.orientation, major.rotation,
		major, minor);
	return result;
}

/**
merging a revolute (ooi) and a spherical (iii) constraint results in:
<ul>
<li> revolute : origins are coincident
<li> fixed : all other cases
</ul>
*/
Joint meld_ooi_iii(const Joint& pin, const Joint& ball, const bool flip = false) {
	//log4cpp::CategoryStream& log = pin.log_cf.infoStream();
	//log << (flip ? "meld_iii_ooi" : "meld_ooi_iii");

	// 
	isis_LOG(lg, isis_FILE, isis_INFO) << (flip ? "meld_iii_ooi" : "meld_ooi_iii");

	e3ga::vector x1 = pin.location;
	e3ga::vector x2 = ball.location;

	e3ga::vector x1x2 = x2 - x1;
	if ( e3ga::zero(x1x2, DEFAULT_TOLERANCE) ) {
		return pin;
	}
	return pin.make_composite(ball);
}
Joint meld_iii_ooi(const Joint& ball, const Joint& pin) {
	return meld_ooi_iii(pin, ball, true);
}

/**
merging a revolute (ooi) and a universal (oii) constraint results in:
<ul>
<li> fixed : all other cases
</ul>
*/
Joint meld_oof_oii(const Joint& axis, const Joint& universal, const bool flip = false) {
	//log4cpp::CategoryStream& log = axis.log_cf.infoStream();
	//log << (flip ? "meld_oii_oof" : "meld_oof_oii");

	// 
	isis_LOG(lg, isis_FILE, isis_INFO) <<  (flip ? "meld_oii_oof" : "meld_oof_oii");

	return axis.make_composite(universal);
}
Joint meld_oii_oof(const Joint& universal, const Joint& axis) {
	return meld_oof_oii(axis, universal, true);
}

/**
merging a revolute (ooi) and a prismatic (oox) constraint results in:
<ul>
<li> fixed : all cases
</ul>
*/
Joint meld_ooi_oox(const Joint& pin, const Joint& slider, const bool flip = false) {
	//log4cpp::CategoryStream& log = pin.log_cf.infoStream();
	//log << (flip ? "meld_oox_ooi" : "meld_ooi_oox");

	// 
	isis_LOG(lg, isis_FILE, isis_INFO) << (flip ? "meld_oox_ooi" : "meld_ooi_oox");

	Joint result(FIXED, 
		pin.location, pin.orientation, pin.rotation,
		pin, slider);
	return result;
}
Joint meld_oox_ooi(const Joint& slider, const Joint& pin) {
	return meld_ooi_oox(pin, slider, true);
}

/**
merging a revolute (ooi) and an axis (oof) may result in:
<ul>
<li> a revolute : coaxial
<li> fixed : all other cases
</ul>
In order to be revolute the axes must be coincident.
*/
Joint meld_ooi_oof(const Joint& pin, const Joint& axis, const bool flip = false) {
	//log4cpp::CategoryStream& log = pin.log_cf.infoStream();
	//log << (flip ? "meld_oof_ooi" : "meld_ooi_oof");

	// 
	isis_LOG(lg, isis_FILE, isis_INFO) << (flip ? "meld_oof_ooi" : "meld_ooi_oof");

	e3ga::vector b1 = e3ga::unit(axis.orientation);
	e3ga::vector b2 = e3ga::unit(pin.orientation);

	e3ga::bivector b1b2 = b1 ^ b2;
	if (! e3ga::zero(b1b2, DEFAULT_TOLERANCE) ) {
		// axes *not* parallel
		// The rotation angle will be the angle between the two axes.
		Joint result(FIXED, 
			pin.location, pin.orientation, cos(b1b2)/Joint::isis_PI,
			pin, axis);
		return result;
	}
	e3ga::vector l1 = e3ga::unit(axis.location);
	e3ga::vector l2 = e3ga::unit(pin.location);

	e3ga::vector l0 = l2 - l1;
	if (e3ga::zero(l0, DEFAULT_TOLERANCE)) {
		// axes strictly coincident
		return pin;
	}
	e3ga::bivector l0b1 = l0 ^ b1;
	if (e3ga::zero(l0b1, DEFAULT_TOLERANCE)) {
		// axes coincident with different locations
		return pin;
	}
	// parallel axes but offset
	// The rotation angle will be that required to
	// bring the axis into position.
	Joint result(FIXED, 
		pin.location, pin.orientation, pin.rotation,
		pin, axis);
	return result;
}
Joint meld_oof_ooi(const Joint& axis, const Joint& pin) {
	return meld_ooi_oof(pin, axis, true);
}

/**
merging two spherical (iii) joints may result in...
<ul>
</ul>
*/
Joint meld_iii_iii(const Joint& major, const Joint& minor) {
	//log4cpp::CategoryStream& log = major.log_cf.infoStream();
	//log << "meld_iii_iii";

	// 
	isis_LOG(lg, isis_FILE, isis_INFO) <<  "meld_iii_iii";

	return major.make_composite(minor);
}

/**
merging a pair of universal (oii) joints:
<ul>
<li> fixed : all other cases
</ul>
*/
Joint meld_oii_oii(const Joint& major, const Joint& minor) {
	//log4cpp::CategoryStream& log = major.log_cf.infoStream();
	//log << "meld_oii_oii";

	// 
	isis_LOG(lg, isis_FILE, isis_INFO) << "meld_oii_oii";

	return major.make_composite(minor);
}

/**
merging a universal (oii) and spherical (iii) constraint may result in:
<ul>
<li> fixed : all other cases
</ul>
*/
Joint meld_oii_iii(const Joint& universal, const Joint& ball, const bool flip = false) {
	//log4cpp::CategoryStream& log = universal.log_cf.infoStream();
	//log << (flip ? "meld_iii_oii" : "meld_oii_iii");

	// 
	isis_LOG(lg, isis_FILE, isis_INFO) << (flip ? "meld_iii_oii" : "meld_oii_iii");
	return universal.make_composite(ball);
}
Joint meld_iii_oii(const Joint& ball, const Joint& universal) {
	return meld_oii_iii(universal, ball, true);
}

/**
merging a universal (oii) and prismatic (oox) constraint may result in:
<ul>
<li> fixed : all other cases
</ul>
*/
Joint meld_oii_oox(const Joint& universal, const Joint& slider, const bool flip = false) {
	//log4cpp::CategoryStream& log = universal.log_cf.infoStream();
	//log << (flip ? "meld_oox_oii" : "meld_oii_oox");

	// 
	isis_LOG(lg, isis_FILE, isis_INFO) << (flip ? "meld_oox_oii" : "meld_oii_oox");

	return universal.make_composite(slider);
}
Joint meld_oox_oii(const Joint& slider, const Joint& universal) {
	return meld_oii_oox(universal, slider, true);
}

/**
merging a universal (oii) and planar (xxi) constraint may result in:
<ul>
<li> fixed : all other cases
</ul>
*/
Joint meld_oii_xxi(const Joint& universal, const Joint& planar, const bool flip = false) {
	//log4cpp::CategoryStream& log = universal.log_cf.infoStream();
	//log << (flip ? "meld_xxi_oii" : "meld_oii_xxi");

	// 
	isis_LOG(lg, isis_FILE, isis_INFO) << (flip ? "meld_xxi_oii" : "meld_oii_xxi");

	return universal.make_composite(planar);
}
Joint meld_xxi_oii(const Joint& planar, const Joint& universal) {
	return meld_oii_xxi(universal, planar, true);
}

/**
merging a spherical (iii) and prismatic (oox) constraint may result in:
<ul>
<li> fixed : all other cases
</ul>
*/
Joint meld_iii_oox(const Joint& ball, const Joint& slider, const bool flip = false) {
	//log4cpp::CategoryStream& log = ball.log_cf.infoStream();
	//log << (flip ? "meld_oox_iii" : "meld_iii_oox");

	// 
	isis_LOG(lg, isis_FILE, isis_INFO) << (flip ? "meld_oox_iii" : "meld_iii_oox");

	return ball.make_composite(slider);
}
Joint meld_oox_iii(const Joint& slider, const Joint& ball) {
	return meld_iii_oox(ball, slider, true);
}

/**
merging a spherical (iii) and cylindrical (oof) constraint may result in:
<ul>
<li> fixed : all other cases
</ul>
*/
Joint meld_iii_oof(const Joint& ball, const Joint& axis, const bool flip = false) {
	//log4cpp::CategoryStream& log = ball.log_cf.infoStream();
	//log << (flip ? "meld_oof_iii" : "meld_iii_oof");

	// 
	isis_LOG(lg, isis_FILE, isis_INFO) << (flip ? "meld_oof_iii" : "meld_iii_oof");

	return ball.make_composite(axis);
}
Joint meld_oof_iii(const Joint& axis, const Joint& ball) {
	return meld_iii_oof(ball, axis, true);
}

/**
merging a prismatic (oox) and planar (xxi) constraint may result in:
<ul>
<li> prismatic : if the new plane is not independent of the slider
<li> fixed : all other cases
</ul>
*/
Joint meld_oox_xxi(const Joint& slider, const Joint& planar, const bool flip = false) {
	//log4cpp::CategoryStream& log = slider.log_cf.infoStream();
	//log << (flip ? "meld_xxi_oox" : "meld_oox_xxi");

	// 
	isis_LOG(lg, isis_FILE, isis_INFO) << (flip ? "meld_xxi_oox" : "meld_oox_xxi");

	    // the basis vector for the slider 
    e3ga::vector w = e3ga::unit(slider.orientation);
    e3ga::vector v = e3ga::unit(planar.orientation);

    // Is the plane parallel to the axis of the slider?
    if ( e3ga::zero(w % v, DEFAULT_TOLERANCE) ) {
        return slider;
    }
    // the bivector for the planar
    e3ga::bivector uv = *v;

	// two basis vectors for the slider bivectors
	if (e3ga::zero(w ^ *uv, DEFAULT_TOLERANCE))
	{

	}
	// only the location point needs to be locked down.
	// x = [(p ^ v ^ w)/(u ^ v ^ w)]u + 
	//      (q ^ u ^ w)/(u ^ v ^ w)]v + 
	//      (r ^ u ^ v)/(u ^ v ^ w)]w
	// where 
	//     p,q,r are a location points in the thre planes
	//     u,v,w are basis vectors for the three planes.
	//  p is on v^w ; q is on u^w ; and r is on u^v
	
	// two basis vectors for the slider bivectors
	e3ga::vector  u = e3ga::unit(w << uv);
//	e3ga::vector  v = e3ga::unit(u << uv);

	e3ga::bivector vw = v ^ w;
	e3ga::bivector wu = w ^ u;

	e3ga::vector  p = slider.location; 
	e3ga::vector  q = p;
	e3ga::vector  r = planar.location;

	double uvw = norm(u ^ v ^ w);

	double pvw = norm(p ^ v ^ w);
	double quw = norm(q ^ u ^ w);
	double ruv = norm(r ^ u ^ v);

	e3ga::vector location = 
		(pvw / uvw)*u + (quw / uvw) * v + (ruv / uvw) * w;
	
	// otherwise the slider information is sufficient.
	Joint result(FIXED, 
		slider.location, slider.orientation, slider.rotation,
		slider, planar);
	return result;

	//return slider.compose(planar);
}
Joint meld_xxi_oox(const Joint& planar, const Joint& slider) {
	return meld_oox_xxi(slider, planar, true);
}

/**
merging a prismatic (oox) and cylindrical (oof) constraint may result in:
<ul>
<li> fixed : all other cases
</ul>
An outer product of the orientation will be zero when they are parallel.
When they define a fixed joint, the locating point is on the
axis of the prismatic joint.
*/
Joint meld_oox_oof(const Joint& slider, const Joint& axis, const bool flip = false) {
	//log4cpp::CategoryStream& log = slider.log_cf.infoStream();
	//log << (flip ? "meld_oof_oox" : "meld_oox_oof");

	// 
	isis_LOG(lg, isis_FILE, isis_INFO) << (flip ? "meld_oof_oox" : "meld_oox_oof");

	e3ga::vector u = e3ga::unit(slider.orientation);
	e3ga::vector v = e3ga::unit(axis.orientation);

	e3ga::bivector uv = u ^ v;
	if (e3ga::zero(uv, DEFAULT_TOLERANCE)) {
		return slider;
	}
	e3ga::vector  w = dual(uv);

	e3ga::bivector vw = v ^ w;
	e3ga::bivector wu = w ^ u;

	double uvw = norm(u ^ v ^ w);

	e3ga::vector  p = axis.location;
	e3ga::vector  q = slider.location; 
	e3ga::vector  r = q;

	double pvw = norm(p ^ v ^ w);
	double quw = norm(q ^ u ^ w);
	double ruv = norm(r ^ u ^ v);

	e3ga::vector location = 
		(pvw / uvw)*u + (quw / uvw) * v + (ruv / uvw) * w;

	Joint result(FIXED, 
		location, slider.orientation, slider.rotation,
		slider, axis);
	return result;
}
Joint meld_oof_oox(const Joint& axis, const Joint& slider) {
	return meld_oox_oof(slider, axis, true);
}


/**
meld the two joints together to produce a new joint.
The number of cases is reduced 
so take care that the major type is greater
than the minor type.
*/
Joint Joint::meld(const Joint& that) const {
	//log4cpp::CategoryStream& log = log_cf.infoStream();

	// 

	switch( that.type ) {
	case FIXED: 
		isis_LOG(lg, isis_FILE, isis_INFO) << "meld_" 
			<< joint_type_code_map[that.type]
			<< "_ooo";
		return that;
	case FREE: 
		isis_LOG(lg, isis_FILE, isis_INFO) << "meld_" 
			<< joint_type_code_map[this->type]
			<< "_fff";
		return *this;
	}

	switch( this->type ) {
	case FIXED:
		isis_LOG(lg, isis_FILE, isis_INFO) << "meld_ooo_" 
			<< joint_type_code_map[this->type];
		return *this;
	case FREE:
		isis_LOG(lg, isis_FILE, isis_INFO) << "meld_fff_" 
			<< joint_type_code_map[that.type];
		return that;
	case REVOLUTE:
		switch( that.type ) {
		case REVOLUTE: 
			return meld_ooi_ooi(*this, that);
		case UNIVERSAL: 
			return meld_ooi_oii(*this, that);
		case SPHERICAL: 
			return meld_ooi_iii(*this, that);
		case PRISMATIC: 
			return meld_ooi_oox(*this, that);
		case PLANAR: 
			return meld_ooi_xxi(*this, that);
		case CYLINDRICAL: 
			return meld_ooi_oof(*this, that);
		default:
			return this->make_composite(that);
		}
	case UNIVERSAL:
		switch( that.type ) {
		case REVOLUTE: 
			return meld_oii_ooi(*this, that);
		case UNIVERSAL: 
			return meld_oii_oii(*this, that);
		case SPHERICAL: 
			return meld_oii_iii(*this, that);
		case PRISMATIC: 
			return meld_oii_oox(*this, that);
		case PLANAR: 
			return meld_oii_xxi(*this, that);
		case CYLINDRICAL: 
			return meld_oii_oof(*this, that);
		default:
			return this->make_composite(that);
		}
	case SPHERICAL:
		switch( that.type ) {
		case REVOLUTE: 
			return meld_iii_ooi(*this, that);
		case UNIVERSAL: 
			return meld_iii_oii(*this, that);
		case SPHERICAL: 
			return meld_iii_iii(*this, that);
		case PRISMATIC: 
			return meld_iii_oox(*this, that);
		case PLANAR: 
			return meld_iii_xxi(*this, that);
		case CYLINDRICAL: 
			return meld_iii_oof(*this, that);
		default:
			return this->make_composite(that);
		}
	case PRISMATIC:
		switch( that.type ) {
		case REVOLUTE: 
			return meld_oox_ooi(*this, that);
		case UNIVERSAL: 
			return meld_oox_oii(*this, that);
		case SPHERICAL: 
			return meld_oox_iii(*this, that);
		case PRISMATIC: 
			return meld_oox_oox(*this, that);
		case PLANAR: 
			return meld_oox_xxi(*this, that);
		case CYLINDRICAL: 
			return meld_oox_oof(*this, that);
		default:
			return this->make_composite(that);
		}
	case PLANAR:
		switch( that.type ) {
		case REVOLUTE: 
			return meld_xxi_ooi(*this, that);
		case UNIVERSAL: 
			return meld_xxi_oii(*this, that);
		case SPHERICAL: 
			return meld_xxi_iii(*this, that);
		case PRISMATIC: 
			return meld_xxi_oox(*this, that);
		case PLANAR: 
			return meld_xxi_xxi(*this, that);
		case CYLINDRICAL: 
			return meld_xxi_oof(*this, that);
		default:
			return this->make_composite(that);
		}
	case CYLINDRICAL:
		switch( that.type ) {
		case REVOLUTE: 
			return meld_oof_ooi(*this, that);
		case UNIVERSAL: 
			return meld_oof_oii(*this, that);
		case SPHERICAL: 
			return meld_oof_iii(*this, that);
		case PRISMATIC: 
			return meld_oof_oox(*this, that);
		case PLANAR: 
			return meld_oof_xxi(*this, that);
		case CYLINDRICAL: 
			return meld_oof_oof(*this, that);
		default:
			return this->make_composite(that);
		}
	default:
		return this->make_composite(that);
	}
}


/**
Given a set of primative datum joints meld them together to
infer an aggregate joint.
*/
Joint infer_joint(std::vector<Joint> joints) {
	Joint initial;
	for each ( Joint joint in joints ) {
		initial = initial.meld(joint);
	}
	return initial;
}

Joint::pair_t infer_joint_pair(std::vector<Joint::pair_t> joints) {
	Joint::pair_t initial;
	for each ( Joint::pair_t joint_pair in joints ) {
		initial.first = initial.first.meld(joint_pair.first);
		initial.second = initial.second.meld(joint_pair.second);
		//

		// 


		isis_LOG(lg, isis_FILE, isis_DEBUG) << "initial.first.type:    " << JointType_string(initial.first.type);
		isis_LOG(lg, isis_FILE, isis_DEBUG) << "initial.first.location:" << initial.first.location.c_str_e20();
		isis_LOG(lg, isis_FILE, isis_DEBUG) << "initial.first.orientation:" << initial.first.orientation.c_str_e20();
		isis_LOG(lg, isis_FILE, isis_DEBUG) << "initial.second.type:   " << JointType_string(initial.second.type);
		isis_LOG(lg, isis_FILE, isis_DEBUG) << "initial.second.location:" << initial.second.location.c_str_e20();
		isis_LOG(lg, isis_FILE, isis_DEBUG) << "initial.second.orientation:" << initial.second.orientation.c_str_e20();
	}
	return initial;
}

void Junction::update( const Joint::pair_t in_pair ) {
	switch (state) {
	case UNSPECIFIED:
	case INFERRED:
		state = INFERRED;
		joint_pair = in_pair;
		return;
	}
	if( joint_pair.first.type == in_pair.first.type ) {
		state = DIFFERENT;
		return;
	}
	if( joint_pair.second.type == in_pair.second.type ) {
		state = DIFFERENT;
		return;
	}
	if( e3ga::equals
		(joint_pair.first.location, in_pair.first.location,
		DEFAULT_TOLERANCE) ) 
	{
		state = DIFFERENT;
		return;
	}
	if( e3ga::equals
		(joint_pair.second.location, in_pair.second.location,
		DEFAULT_TOLERANCE) ) 
	{
		state = DIFFERENT;
		return;
	}
	if( e3ga::equals
		(joint_pair.first.orientation, in_pair.first.orientation,
		DEFAULT_TOLERANCE) ) 
	{
		state = DIFFERENT;
		return;
	}
	if( e3ga::equals
		(joint_pair.second.orientation, in_pair.second.orientation,
		DEFAULT_TOLERANCE) ) 
	{
		state = DIFFERENT;
		return;
	}
	state = MATCH;
	return;
}

/**
This behavior can be mostly replicated by using the maps.
return joint_type_code_map[in_JointType]
or 
return joint_type_name_map[in_JointType]
*/
std::string JointType_string ( JointType in_JointType)
{

	switch ( in_JointType )
	{
		case  COMPOSITE:
			return "COMPOSITE";
			break;
		case  FIXED:
			return "FIXED";
			break;
		case  REVOLUTE:
			return "REVOLUTE";
			break;
		case  UNIVERSAL:
			return "UNIVERSAL";
			break;
		case  SPHERICAL:
			return "SPHERICAL";
			break;
		case  PRISMATIC:
			return "PRISMATIC";
			break;
		case  PLANAR:
			return "PLANAR";
			break;
		case  CYLINDRICAL:
			return "CYLINDRICAL";
			break;
		case  FREE:
			return "FREE";
			break;
		default:
			return "UNKNOWN";
	}

}

/**
a helper function for creating motor-pair.
*/
double axis_value(const double a11, const double a22, const double a33) {
	return 0.5 * sqrt(1.0 + a11 + a22 + a33);
}


/**
see "Geometric Algebra for Computer Science" 17.10.
The columns of a (rotation) matrix are the images of the
basis vectors under the transformation.
To convert from rotor to matrix, we transform e1, 
e2 and e3 and copy them into the matrix.
*/
void rotor_to_matrix(const e3ga::rotor &r, double m[9]) {
	double qw = _double(r);
	double qx = -r.get_e2_e3();
	double qy = -r.get_e3_e1();
	double qz = -r.get_e1_e2();

	m[0 * 3 + 0] = 1.0f - 2.0f * qy * qy - 2.0f * qz * qz;
    m[1 * 3 + 0] = 2.0f * (qx * qy + qz * qw);
    m[2 * 3 + 0] = 2.0f * (qx * qz - qy * qw);

    m[0 * 3 + 1] = 2.0f * (qx * qy - qz * qw);
    m[1 * 3 + 1] = 1.0f - 2.0f * qx * qx - 2.0f * qz * qz;
    m[2 * 3 + 1] = 2.0f * (qy * qz + qx * qw);

    m[0 * 3 + 2] = 2.0f * (qx * qz + qy * qw);
    m[1 * 3 + 2] = 2.0f * (qy * qz - qx * qw);
    m[2 * 3 + 2] = 1.0f - 2.0f * qx * qx - 2.0f * qy * qy;
}

/**
These functions compute the smallest rotor that
rotates a unit vector 'a' to another unit vector 'b', in 3D.
R = (1 + b a) / sqrt(2*(1+ b.a))
*/
// improved rotorFromVectorToVector due to Allen Cortzen
e3ga::rotor create_rotor(const e3ga::vector &v1, const e3ga::vector &v2) {
    if (e3ga::sp(v1, v2) < -0.9f) {
        // "near" 180 degree rotation: 
        //v1 factor in returning blade regardless of any loss of precision
        e3ga::vector w0 = v1 << (v1^v2);
        double n2 = e3ga::norm2(w0);
        if (n2 == 0.0f){
            e3ga::vector w1 = v1 << (v1^e3ga::vectorE1);
            e3ga::vector w2 = v1 << (v1^e3ga::vectorE2);
            e3ga::vector w = (e3ga::norm2(w1) > e3ga::norm2(w2)) 
				? w1 : w2; 
			return e3ga::exp(v1^e3ga::unit(w));
        }
        //v1 replaced with -v1 and additional 180 degree rotation: 
        double s = sqrt(2.0 * (1.0 - (v2 << v1)));
		return e3ga::exp(e3ga::bivector( (1.0 - v2 * v1) * (1.0f / s)*e3ga::unit(w0), 0));
    }
    double s = sqrt(2.0 * (1.0f + (v2 << v1)));
    return e3ga::exp(e3ga::bivector((1.0 + v2 * v1) * (1.0f / s), 0));
}

e3ga::rotor create_rotor(const e3ga::vector &v1, const e3ga::vector &v2, const e3ga::bivector &rotPlane) {
    if (e3ga::sp(v1, v2) < -0.9f) {
        //v1 replaced with -v1 and additional 180 degree rotation: 
        double s = sqrt(2.0 * (1.0f - (v2 << v1)));
		return e3ga::exp(e3ga::bivector( (1.0 - v2 * v1) * (1.0f / s)*e3ga::unit(rotPlane), 0));
    }
    else {
        double s = ::sqrt(2.0 * (1.0f + (v2 << v1)));
        return e3ga::exp(e3ga::bivector((1.0 + v2 * v1) * (1.0f / s), 0));
    }
}

/**
1) compute the smallest rotor r1, that rotates e1 to
the image of e1 under the matrix transform.
2) compute the smallest rotor r2 that rotates r1 e1/r1
to is image of e2 under the matrix transform.
orthogonality leaves r1 e1/r1 unchanged.
3) compute the full rotor: r = r2 r1
*/
e3ga::rotor matrix_to_rotor(const double in_m[9]) {
	const double (&m)[3][3] = *reinterpret_cast<const double (*)[3][3]>(in_m);
	double trace =  m[0][0] + m[1][1] + m[2][2] + 1.0;
	double qw;
	double qx;
	double qy;
	double qz;
	if (trace > DEFAULT_TOLERANCE) {
		double s = 0.5 / ::sqrt(trace);
		qw = 0.5 * ::sqrt(trace);
		qx = s * (m[2][1] - m[1][2]);
		qy = s * (m[0][2] - m[2][0]);
		qz = s * (m[1][0] - m[0][1]);
	} else {
		if (m[0][0] > m[1][1] && m[0][0] > m[2][2]) {
			double s = 2.0 / ::sqrt(1.0 + m[0][0] - m[1][1] - m[2][2]);
			qw = (m[1][2] - m[2][1]) / s;
			qx = 0.25 * s;
			qy = (m[0][1] + m[1][0]) / s;
			qz = (m[0][2] + m[2][0]) / s;

		} else if (m[1][1] > m[2][2]) {
			double s = 2.0 / ::sqrt(1.0 - m[0][0] - m[1][1] - m[2][2]);
			qw = (m[0][2] - m[2][0]) / s;
			qx = (m[0][1] + m[1][0]) / s;
			qy = 0.25 * s;
			qz = (m[1][2] + m[2][1]) / s;

		} else {
			double s = 2.0 / ::sqrt(1.0 - m[0][0] - m[1][1] + m[2][2]);
			qw = (m[0][1] - m[1][0]) / s;
			qx = (m[0][2] + m[2][0]) / s;
			qy = (m[1][2] + m[2][1]) / s;
			qz = 0.25 * s;
		}
	}
	double s = ::sqrt(qw*qw + qx*qx + qy*qy + qz*qz);
	return e3ga::rotor(e3ga::rotor::coord_scalar_e1e2_e2e3_e3e1,
		qw/s, -qz/s, -qx/s, -qy/s);
}

/**
The in_matrix has the form:
 [ r11 r12 r13 0
   r21 r22 r23 0
   r31 r32 r33 0
   x1  x2  x3  1 ]
Note this is not the standard affine transform position
but rather it is the form provided by Creo.
Also, keep in mind that C using [col][row] indexing 
which is opposite that of typical mathematical notation.
http://arxiv.org/pdf/math/0701759v1.pdf
section 3.1 Determination fo the Euler parameters for a given 3D rotation
*/
MotorPair MotorPair::create(double in_m[4][4]) {
	e3ga::vector offset(e3ga::vector::coord_e1_e2_e3,
		in_m[3][0], in_m[3][1], in_m[3][2]);

	// compute the rotation vector values
	double w = axis_value(  in_m[0][0],  in_m[1][1],  in_m[2][2]);
	double i = axis_value(  in_m[0][0], -in_m[1][1], -in_m[2][2]);
	double j = axis_value( -in_m[0][0],  in_m[1][1], -in_m[2][2]);
	double k = axis_value( -in_m[0][0], -in_m[1][1],  in_m[2][2]);
	if (! e3ga::zero(w, DEFAULT_TOLERANCE)) {
		i *= (in_m[1][2] < in_m[2][1]) ? -1 : 1;
		j *= (in_m[2][0] < in_m[0][2]) ? -1 : 1;
		k *= (in_m[0][1] < in_m[1][0]) ? -1 : 1;
	} else {
		if (! e3ga::zero(i, DEFAULT_TOLERANCE)) {
			j *= (in_m[0][1] < -in_m[1][0]) ? -1 : 1;
			k *= (in_m[2][0] < -in_m[0][2]) ? -1 : 1;
		} else 
		if (! e3ga::zero(j, DEFAULT_TOLERANCE)) {
			i *= (in_m[0][1] < -in_m[1][0]) ? -1 : 1;
			k *= (in_m[1][2] < -in_m[2][1]) ? -1 : 1;
		} else
		if (! e3ga::zero(k, DEFAULT_TOLERANCE)) {
			i *= (in_m[2][0] < -in_m[0][2]) ? -1 : 1;
			j *= (in_m[1][2] < -in_m[2][1]) ? -1 : 1;
		} // else error condition
	}

	e3ga::vector rotate_vector(e3ga::vector::coord_e1_e2_e3, i,j,k);

	e3ga::rotor rotor = exp(e3ga::dual(rotate_vector));
	MotorPair motor(rotor, offset);
	return motor;
}

Joint MotorPair::move(const Joint& in_j) {
	e3ga::vector new_orient = 
		e3ga::applyVersor(rotor,in_j.orientation);

	e3ga::vector new_loc =  offset +
		e3ga::applyVersor(rotor, in_j.location);

	Joint result(in_j.type, new_loc, new_orient, in_j.rotation, 0);
	return result;
}

} // cad
} // isis
