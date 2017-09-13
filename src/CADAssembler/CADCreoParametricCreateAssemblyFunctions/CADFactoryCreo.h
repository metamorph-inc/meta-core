
#ifndef CAD_FACTORY_CREO_H
#define CAD_FACTORY_CREO_H

/**
This class provides factories for the
concrete Creo CAD system.
*/
#include "CadFactoryAbstract.h"
#include <vector>
#include <ProSolid.h>

namespace isis {
namespace cad {
namespace creo {

class AssemblerCreo : public IAssembler {
private:
	ProSolid  assembly_model;

public:
	// provide the name of the concrete assembler
	std::string name() {
		return "Creo Assembler";
	}

	// std::vector<Joint> get_joints();

	/**
	For a particular component (specified by id) extract
	the set of joints which join that component to other
	components.
	*/
	std::vector< Joint::pair_t >  extract_joint_pair_vector
		(const std::string in_component_id,
		 std::vector<ConstraintPair> in_vector,
		 std::map<std::string, isis::CADComponentData> &	in_map);

	/**
	The void pointer is CAD system specific.
	It should be eliminated as the strategy is completed.
	*/
	void* get_assembly_component
		( const std::string in_working_directory,
		  const std::string &	in_id, 
		  std::map<std::string, isis::CADComponentData> &	in_map);



	//virtual void SetupCADEnvirnoment ( const DataContainer &in_DataContainer) throw (isis::application_exception);

};

class CadFactoryCreo : public CadFactoryAbstract
{
private:
	AssemblerCreo assembler;

public:
	// provide the name of the concrete factory
	std::string name() {
		return "Creo CAD Factory";
	}
	IAssembler& get_assembler() {
		return assembler;
	};

};

CadFactoryAbstract::ptr create();


} // creo
} // cad
} // isis

#endif // CAD_FACTORY_CREO_H