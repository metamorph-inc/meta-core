
#include <TemplateModel.h>

#include <ProMdlUnits.h>
#include <ProUtil.h>
#include <ProWstring.h>
#include "cc_LoggerBoost.h"

namespace isis {
namespace creo {
namespace model {

bool make_solid_templated( ProSolid& in_original, ProSolid& out_template )
{
	ProError rc;
	char pro_str[128];
	
	
	ProUnitsystem original_system;
	switch( rc = ProMdlPrincipalunitsystemGet(in_original, &original_system) ) {
	case PRO_TK_NO_ERROR: break;
	case PRO_TK_BAD_INPUTS:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) 
			<< "failed getting the principal unit-system.";
		break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) 
			<< "could not aquire the unit system = " << rc;
		return false;
	}
	/*
	ProUnititem* original_mass_unit;
	switch( rc = ProMdlUnitsCollect( in_original, 
		PRO_UNITTYPE_MASS, &original_mass_unit) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not collect unit systems = " << rc;
		return false;
	}
	*/

	ProUnitsystem* template_systems;
	//ProUnititem template_units;
	switch( rc = ProMdlUnitsystemsCollect( out_template, &template_systems) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not collect unit systems = " << rc;
		return false;
	}
	ProUnitsystem template_system;
	int size_template_systems;
	ProArraySizeGet( template_systems, &size_template_systems );
	for( int ix=0; ix < size_template_systems; ++ix ) {
		int names_matched;
		ProWstringCompare(template_systems[ix].name,
			original_system.name, PRO_VALUE_UNUSED, &names_matched);
		if (names_matched != 0) continue;
		template_system = template_systems[ix];
		break;
	}

	switch( rc = ProMdlPrincipalunitsystemSet(out_template, 
		&template_system, PRO_UNITCONVERT_SAME_SIZE, PRO_B_TRUE,
		PRO_VALUE_UNUSED) ) {
	case PRO_TK_NO_ERROR: break;
	case PRO_TK_BAD_INPUTS:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) 
			<< "could not set the units in the shrinkwrap : "
			<< (const char*)ProWstringToString(pro_str, template_system.name);
		break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) 
			<< "could not set units in shinkwrap = "
			<< rc;
		return false;
	}
	return true;
}

} // model
} // creo
} // isis