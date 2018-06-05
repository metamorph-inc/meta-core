#ifndef CC_MULTI_FORMAT_STRING_H
#define CC_MULTI_FORMAT_STRING_H

#include "isis_application_exception.h"
#include <string>
#include <iostream>


namespace isis
{

	const int CAD_STRING_DO_NOT_CHECK_NUM_CHARS = -999999;

	// Description:
	//	This function creates a representation of a string based on passing a constructor one of the following types:
	//		char *
	//		std::string
	//		wchar_t
	//	Conversion operators will return any of the three types.  Internally to this class, all three data types are
	//	persisted. 
	//
	//	Pre-Conditions:
	//		in_charArray (both char and wchar) must be null terminated char arrays.
	//		The input strings can be zero length strings.
	//
	//	Post-Conditions:
	//		Conversion operators return any of the three types.
	//		Assignment operators do a deep copy, the lvalue is independent of the rvalue
	//		The destructor deletes the char/wchar_t arrays stored on the heap.
	//
	//	Thread Safety:
	//		The conversion operators are thread safe.  The assignment operators are NOT thread safe.  If objects from 
	//		this class are going to be used in multiple threads, then instantiate the object with the required 
	//		char/wchar/string data, and after that only use the conversion operators.
	class MultiFormatString
	{	
		public:

			MultiFormatString( 
						int	in_MaxNumberChars = CAD_STRING_DO_NOT_CHECK_NUM_CHARS);
			MultiFormatString( const	std::string &in_String,
						int		in_MaxNumberChars = CAD_STRING_DO_NOT_CHECK_NUM_CHARS);
			MultiFormatString( const char		*in_charArray,
						int		in_MaxNumberChars = CAD_STRING_DO_NOT_CHECK_NUM_CHARS);
			MultiFormatString( const wchar_t	*in_charArray,
						int		in_MaxNumberChars = CAD_STRING_DO_NOT_CHECK_NUM_CHARS);

			// The following line will invoke "operator const char* ()"
			// std::cout << std::endl << MultiFormatString_1;
			// cout favors char* over std::string.
			operator const char* () const;

			// The following line will invoke "operator const wchar_t* ()"
			operator const wchar_t* () const;

			// The following line will invoke "operator const std::string ();"
		    // std::cout << std::endl << (std::string)MultiFormatString_1;
			// The cast (std::string) is necessary because by default cout favors char*.
			// If you know that MultiFormatString was created with a std::string, then use the cast (std::string)
			// so that an internal conversion to a char* would not be necessary.
			operator const std::string& () const;

			// This is declared but not defined to cause an error if you do MultiFormatString == "string literal". Do static_cast<const std::string&>(mfs) == "string literal" or use strcmp instead.
			bool operator ==(const char* b) const;

			MultiFormatString& operator=( const char* );
			MultiFormatString& operator=( const wchar_t* );
			MultiFormatString& operator=( const std::string& );
			MultiFormatString& operator=( const	MultiFormatString &in_MultiFormatString );	

			MultiFormatString( const MultiFormatString &in_MultiFormatString); 

			// size is the number of characters.  The buffer sizes are size + 1
			unsigned int size() const;

			~MultiFormatString();

		private:
			std::string		narrowString;
			bool			narrowString_Defined;		
			char			*narrowCharArray;
			wchar_t			*wideCharArray;
			int				maxNumberChars;

			void setString(const char* );
			void setString(const wchar_t* );
			void setString(const std::string& );

			void setNarrowString();
			void setNarrowCharArray();
			void setWideCharArray();

			void setAllStrings();

			void deleteAndNullStringValues();
			void nullStringValues();

	};


} //END namespace isis

#endif