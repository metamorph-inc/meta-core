/*! \file isis_application_exception.h
    \brief Wrapper class for std::exception.

	isis::application_exception wraps std::exception so that exceptions can be caught based on 
	isis::application_exception.  This provides for segregating catches of   
	isis::application_exception and std::exception.
*/
#ifndef ISIS_APPLICATION_EXCEPTION_H
#define ISIS_APPLICATION_EXCEPTION_H

#include <iostream>
#include <sstream>
#include <exception>

namespace isis
{
	//!  application_exception class. 
	/*!
	A wrapper class for std::exception.  application_exception would normally be thrown by
	CAD routines developed by ISIS. 
	*/

	class application_exception : public std::exception
	{
		public:
			application_exception(const char *in_Message)  throw() ;
			application_exception(const std::string & in_Message) throw() ;
			application_exception(const std::stringstream & in_Message) throw() ;

			application_exception(const char *in_code, const char *in_Message) throw();
			application_exception(const char *in_code, const std::string & in_Message) throw() ;
			application_exception(const char *in_code, const std::stringstream & in_Message) throw() ;

			char *get_error_code() { return error_code; }
			virtual ~application_exception() throw();
			std::string tostring() const;
			void setComponentInfo(const std::string &s)
			{
				componentinfo = s;
			}
			std::string getComponentInfo()
			{
				return componentinfo;
			}
			void setDatumInfo(const std::string &s)
			{
				datuminfo = s;
			}
			std::string getDatumInfo()
			{
				return datuminfo;
			}
			void setParameterInfo(const std::string &s)
			{
				parameterinfo = s;
			}
			std::string getParameterInfo()
			{
				return parameterinfo;
			}
	private:
		char error_code[8];
		std::string componentinfo; // possible information about the component
		std::string parameterinfo; // possible information about the parameter
		std::string datuminfo; // possible information about the datum
	};

	std::ostream& operator <<(std::ostream & out, const application_exception & applEx);
	
}

#endif  /* APPLICATION_EXCEPTION_H */