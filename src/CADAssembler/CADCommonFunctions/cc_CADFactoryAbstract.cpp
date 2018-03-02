#include "cc_CADFactoryAbstract.h"


namespace isis {
namespace cad {

CadFactoryAbstract_global *CadFactoryAbstract_global::s_instance = 0;


CadFactoryAbstract_global* CadFactoryAbstract_global::instance()
{
	if ( !s_instance)
		s_instance = new CadFactoryAbstract_global();
	return s_instance;

}

// Should call this function one and only one time
void CadFactoryAbstract_global::setCadFactoryAbstract_ptr(isis::cad::CadFactoryAbstract::ptr in_cADFactory_ptr ) throw (isis::application_exception)
{

	if ( factorySet )
	{
		std::stringstream errorString;
		errorString <<	"Function - " << __FUNCTION__ << ", " << std::endl <<
						"This function should be called one and only one time.  It has been previously called. Remove the redundant invocations from the code.";
		throw isis::application_exception(errorString);		

	}

	cADFactory_ptr = in_cADFactory_ptr;
	factorySet = true;
}

CadFactoryAbstract::ptr CadFactoryAbstract_global::getCadFactoryAbstract_ptr() throw (isis::application_exception)
{
	if ( factorySet )
	{
		return  cADFactory_ptr;
	}
	else
	{
		std::stringstream errorString;
		errorString <<	"Function - " << __FUNCTION__ << ", " << std::endl <<
						"Prior to invoking this function, the setCadFactoryAbstract_ptr function must be called";
		throw isis::application_exception(errorString);		

	}
}


MultiFormatString ICADSession::getCADWorkingDirectory () throw (isis::application_exception)
{

	if ( cADWorkingDirectory.size() == 0 )
	{
		std::stringstream errorString;
		errorString <<	"Function - " << __FUNCTION__ << ", " << std::endl <<
						"Null value for ICADSession::cADWorkingDirectory. Prior to invoking this function, the ICADSession::setCADWorkingDirectory function must be called with a non null value.";
		throw isis::application_exception(errorString);		
	}
	return cADWorkingDirectory;
}


} // cad
} // isis