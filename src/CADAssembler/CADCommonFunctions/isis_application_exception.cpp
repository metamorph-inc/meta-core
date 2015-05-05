#include "isis_application_exception.h"
#include <string.h>
#include <sstream>

namespace isis
{
	application_exception::application_exception(const char *in_Message) throw() 
		: exception(in_Message) 
	{ strncpy_s(error_code, sizeof(error_code)-1, "C000000", _TRUNCATE ); }

	application_exception::application_exception(const std::string & in_Message) throw() 
		: exception(in_Message.c_str())  
	{ strncpy_s(error_code, sizeof(error_code)-1, "C000000", _TRUNCATE ); }

	application_exception::application_exception(const std::stringstream & in_Message) throw() 
		: exception(in_Message.str().c_str())  
	{ strncpy_s(error_code, sizeof(error_code)-1, "C000000", _TRUNCATE ); }


	application_exception::application_exception(const char *in_code, const char *in_Message) throw() 
		: exception(in_Message)  
	{ strncpy_s(error_code, sizeof(error_code)-1, "C000000", _TRUNCATE ); }

	application_exception::application_exception(const char *in_code, const std::string & in_Message) throw() 
		: exception(in_Message.c_str())  
	{ strncpy_s(error_code, sizeof(error_code)-1, "C000000", _TRUNCATE ); }

	application_exception::application_exception(const char *in_code, const std::stringstream & in_Message) throw() 
		: exception(in_Message.str().c_str())  
	{ strncpy_s(error_code, sizeof(error_code)-1, "C000000", _TRUNCATE ); }


	application_exception::~application_exception() throw(){};

	std::string application_exception::tostring() const {
		std::ostringstream str;
		str << this->what();
		if (componentinfo.size()>0)
			str << " " << "COMPONENT: " << componentinfo;
		if (datuminfo.size() > 0)
			str << " " << "DATUM: " << datuminfo;
		if (parameterinfo.size() > 0)
			str << " " << "PARAM: " << parameterinfo;
		return str.str();
	}

	std::ostream& operator <<(std::ostream & out, const application_exception & applEx) {
		out << (applEx.tostring());
		return out;
	}

}