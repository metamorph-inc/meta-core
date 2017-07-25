#include <iostream>
#include "ProObjects.h"
#include "ProMdl.h"
#include "ProParamval.h"

/**
The ProE source does not include output operators (<<) for the various 
data types making it tedious to write logs containing them.
*/

namespace isis
{
	
	/**
	Should show the names of the status messages.
	Not just the numerical values.
	*/
	std::ostream& operator <<(std::ostream & out, const ProError & status) ;

	std::ostream& operator <<(std::ostream & out, const ProParamvalue & param);

	std::ostream& operator <<(std::ostream & out, const ProModel & model);

	std::ostream& operator <<(std::ostream & out, const ProModelitem & item);

	std::ostream& operator <<(std::ostream & out, const ProMdlType & type);

	std::ostream& operator <<(std::ostream & out, const ProMdl & type);

	std::ostream& operator <<(std::ostream & out, const ProParamowner & owner) ;

	
	std::ostream& operator <<(std::ostream & out, const ProParameter & param) ;

	/**
	see "ProObjects.h"
	*/
	std::ostream& operator <<(std::ostream & out, const ProType & type) ;

}
