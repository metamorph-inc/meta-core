#ifndef _C_CODEGEN_H_
#define _C_CODEGEN_H_


/*** Included Header Files ***/
#include "Uml.h"
#include "ESMoL\generated\ESMoL.h"
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>


// Typedefs for ESMoL Paradigm
typedef std::set< ESMoL::DesignFolder > DesignFolderSet;
typedef std::set< ESMoL::SystemTypes > SystemTypesSet;
typedef std::set< ESMoL::Component > ComponentSet;
typedef std::set< ESMoL::ComponentBase > ComponentBaseSet;
typedef std::set< ESMoL::CCode > CCodeSet;
typedef std::set< ESMoL::CInputPort > InputPortSet;
typedef std::set< ESMoL::COutputPort > OutputPortSet;


/***************************************************************************************/


class CCodeGen {
private:
	bool										_debug;
	std::string TemplatePath( void );
	// Private Methods
	CCodeGen();														//!< Deny access to default constructor
public:
	// Constructors and Destructors	
	CCodeGen( const std::string &outputDir, const std::string &esmolModel, const bool &debug=false );
	~CCodeGen() { }
};


class CCodeGenException {
public:
	CCodeGenException( std::string &reason ) : _reason(reason) { }
	std::string									_reason;
};


/***************************************************************************************/


#endif // _C_CODEGEN_H_

