
#ifndef ABSTRACT_CAD_FACTORY_H
#define ABSTRACT_CAD_FACTORY_H

#include <string>

/**
This abstract class provides factories for the
concrete CAD systems.
*/

namespace isis {
namespace cad {

class IAssembler {
public:
	// provide the name of the concrete assembler
	virtual std::string name() = 0;
};

class AbstractCADFactory {
public:
	// provide the name of the concrete factory
	virtual std::string name() = 0;

	// get the concrete assembler
	virtual IAssembler& getAssembler() = 0;

};

} // cad
} // isis

#endif // ABSTRACT_CAD_FACTORY_H