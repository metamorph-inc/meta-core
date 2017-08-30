/*! \file CommonUtilities.h
    \brief  Common utilities used by Creo CAD applications.

	Common utilities that support operations such as copying, deleting... files/models.
*/

#ifndef COMMON_UTILITIES_H
#define COMMON_UTILITIES_H

#include <isis_ptc_toolkit_functions.h>
#include <isis_application_exception.h>
#include <string>


namespace isis
{

		void DeleteModel_IfItExists( ProFamilyName name, 
								 ProMdlType    type ) throw (isis::application_exception);


} // end namespace isis

#endif