#include <CommonUtilities.h>
#include <fstream>
#include <locale>

//#include <iostream>

namespace isis
{


	void DeleteModel_IfItExists( ProFamilyName name, 
								 ProMdlType    type ) throw (isis::application_exception)
	{
		bool ModelExists = true;
		ProMdl p_handle;
		try
		{
			isis_ProMdlRetrieve( name, type, &p_handle);
		}
		catch (...)
		{
			ModelExists = false;
		}

		if ( ModelExists )
		{
			isis_ProMdlDelete( p_handle );
		}

	}




} // end namespace isis