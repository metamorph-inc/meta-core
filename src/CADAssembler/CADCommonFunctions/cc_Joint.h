
#ifndef JOINT_H
#define JOINT_H


#include <map>
#include <vector>
#include <boost/smart_ptr/shared_ptr.hpp>

#include "e3ga.h"
#include <boost/assign/list_of.hpp>

#include "cc_LoggerBoost.h"
#include "cc_StringToEnumConversions.h"

namespace isis {
namespace cad {

double const DEFAULT_TOLERANCE = 0.001;

/** 
  o : constrained in translation and rotation
  x : free in translation, constrained in rotation
  i : free in rotation, constrained in translation
  f : free in rotation and translation
  The three positions represent the three basis vectors.
  The following are isomorphic:
  oxf = xfo = fox = xof = fxo = oxf 
  When constructing the names the three symbols
  are sorted in the the order [ovalues threare sorted in 
*/
enum JointType {
	COMPOSITE = 0,

	fff = 20, xff = 19, iff = 18, off = 17, 
	xxf = 16, xif = 15, oxf = 14, iif = 13, 
	oif = 12, oof = 11, xxx = 10, xxi =  9,
	oxx =  8, xii =  7, oxi =  6, oox =  5, 
	iii =  4, oii =  3, ooi =  2, ooo =  1,

	FIXED = ooo,
	REVOLUTE = ooi,
	UNIVERSAL = oii,
	SPHERICAL = iii,
	PRISMATIC = oox,
	PLANAR = xxi,
	CYLINDRICAL = oof,
	FREE = fff,

	DATUM_CSYS = ooo,
	DATUM_PLANE = xxi,
	DATUM_AXIS = oof,
	DATUM_POINT = iii,
};

e_CADJointType GetCADJointType( isis::cad::JointType in_JointType);


std::string JointType_string ( JointType in_JointType);

typedef std::map<std::string, JointType> joint_type_map_t;
extern joint_type_map_t joint_type_map;

std::ostream& operator<<( std::ostream& out, JointType& that );

class Joint {
public:
	//log4cpp::Category& log_cf;

	static double const isis_PI;

	typedef ::boost::shared_ptr<Joint> ptr;
	typedef std::pair<Joint, Joint> pair_t;

public:

	// joint type
	const JointType type;
	// location in the components (.prt/.asm) coordinate system
	const e3ga::vector location;
	// orientation in axis representation in the components (.prt/.asm) coordinate system
	const e3ga::vector orientation;
	// absolute rotation about the axis.
	// in pieces of pi [-1.0 to 1.0]
	const double rotation;
	std::vector<const Joint> composite;

public:

	Joint()
		: // log_cf(log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY)),
		type(FREE), location(), 
		orientation(), rotation(0.0),
		composite(0)
	{
	}

	Joint(const Joint& that )
		:// log_cf(log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY)),
		type(that.type), location(that.location), 
		orientation(that.orientation), rotation(that.rotation),
		composite(that.composite)
	{}

	Joint(const Joint& major, const Joint& minor )
		:// log_cf(log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY)),
		type(COMPOSITE), location(major.location), 
		orientation(major.orientation), rotation(major.rotation),
		composite(2)
	{ 
		composite[0] = major;
		composite[1] = minor;
	}

	Joint make_composite(const Joint& minor) const {
		Joint result(*this, minor);
		return result;
	}

	Joint(const JointType in_type, const e3ga::vector in_location,
		const e3ga::vector in_orientation, const double in_rotation,
		const Joint major, const Joint minor)
		: // log_cf(log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY)), 
		type(in_type), location(in_location), 
		orientation(in_orientation), rotation(in_rotation),
		composite(2)
	{ 
		composite[0] = major;
		composite[1] = minor;
	}

	Joint(const JointType in_type, const e3ga::vector in_location,
		const e3ga::vector in_orientation, const double in_rotation,
		int)
		: //log_cf(log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY)), 
		type(in_type), location(in_location), 
		orientation(in_orientation), rotation(in_rotation),
		composite(0)
	{ }

	// get the location.
	e3ga::vector getLocationAbsolute();
	// get the orientation as a set of 313 euler angles.
	e3ga::vector getOrientation313Absolute(); 

	// meld the two joints together to produce a new joint.
	Joint meld(const Joint& in_that) const;
	
	Joint& operator=( const Joint& that );

	std::string toString();
};

std::ostream& operator<<( std::ostream& out, Joint& that );

/**
Given a set of primative datum joints fuse them together to
infer an aggregate joint.
*/
Joint infer_joint(std::vector<Joint> in_joints);

//

// The first in Joint::pair_t would be based on the datums in the assembled .prt/.asm 
// and the second would be the constrained-to datums.  The datums for the assembled 
// .prt/.asm would always be contained in the assembled .prt/.asm; however, the 
// constrained-to datums could be in different .prts/.asms.  This would not be typical use 
// case but would be possible.  For example, a plate could be constrained to more than one 
// other plates.  For the case of multiple constrained-to .prts/.asms, the inferred joint 
// information will not be correct.  This is because the inferred information is based on 
// the .prts/.asms coordinate system, which is irrelevant when spanning .prts/.asms.  
// The correction for this condition would be to infer the joint type based on the global 
// coordinate system (i.e. top-assembly coordinate system). For this to work, the assembled 
// component would have to be constrained based the first Joint::pair_t, and then once 
// assembled determine the second Joint::pair_t.  
Joint::pair_t infer_joint_pair(std::vector<Joint::pair_t> in_joints);

enum JunctionState {
	// specified but not yet compared with inference
	SPECIFIED,    
	// not specified but inferance not yet run
	UNSPECIFIED,  
	// not specified only inferred
	INFERRED,     
	// specified and matches inference
	MATCH,        
	// specified and is consistent with inference
	// this is not as strong as a match
	CONSISTENT,   
	// specified and matches inference
	DIFFERENT,    
};

class Junction {
public:
	JunctionState state;
	isis::cad::Joint::pair_t  joint_pair;

	/**
	The joint pair provided is used to update 
	the junction object.
	*/
	void update( const Joint::pair_t in_pair );
	Junction() : state(UNSPECIFIED){};

};


class MotorPair {
private:
	e3ga::rotor rotor;
	e3ga::vector offset;

	MotorPair(e3ga::rotor in_rotor, e3ga::vector in_offset)
		: rotor(in_rotor), offset(in_offset) {};

public:
	/** 
	Move the joint to its new coordinate frame.
	*/
	Joint move(const Joint& in_joint);

	/**
	Construct a ga-motor from an affine transform matrix.
	*/
	static MotorPair create(double in_matrix[4][4]);
};

} // cad
} // isis

#endif // JOINT_H
