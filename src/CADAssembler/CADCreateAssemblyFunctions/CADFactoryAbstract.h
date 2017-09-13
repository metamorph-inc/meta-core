
#ifndef CAD_FACTORY_ABSTRACT_H
#define CAD_FACTORY_ABSTRACT_H

#include <string>
#include "cc_Joint.h"
#include "cc_CommonStructures.h"
#include "isis_application_exception.h"
#include <boost/smart_ptr.hpp>

/**
This abstract class provides factories for the
concrete CAD systems.
*/

namespace isis {
namespace cad {

	/*
struct  DataContainer 
{
		std::string string_1;
		std::string string_2;
};
*/

class IAssembler {
public:
	// provide the name of the concrete assembler
	virtual std::string name() = 0;

	// virtual std::vector<Joint> get_joints() = 0;
	
	/**
	Given a set of constraints infer the joint.
	*/
	virtual std::vector<Joint::pair_t>  extract_joint_pair_vector
		(const std::string in_component_id,
		 std::vector<ConstraintPair> in_component_pair_vector,
		 std::map<std::string, isis::CADComponentData> &	in_CADComponentData_map)
		 = 0;

	/**
	There is an CAD specific object for the assembly.
	This function sets that object.
	*/
	virtual void* get_assembly_component
		( const std::string in_working_directory,
		  const std::string &	in_id, 
		  std::map<std::string, isis::CADComponentData> &	in_map)
		= 0;


	// This function does any setup that is necessary before invoking the CAD application
	//virtual void SetupCADEnvirnoment ( const DataContainer &in_DataContainer) throw (isis::application_exception) = 0;

};


class CadFactoryAbstract {
public:
	typedef boost::shared_ptr<CadFactoryAbstract> ptr;

	// provide the name of the concrete factory
	virtual std::string name() = 0;

	/**
	Get the CAD specific concrete functor for
	manipulating the assembly.
	*/
	virtual IAssembler& get_assembler() = 0;

};

} // cad
} // isis

#endif // CAD_FACTORY_ABSTRACT_H