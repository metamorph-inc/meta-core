#include "ParseUtils.h"

double to_double( char * strval ) {
	std::string sv( strval );
	double val;

	if( from_string < double >( val, sv, std::dec ) )
		return val;
	else
		return -1.0;
}

long long to_integer( char * strval ) {
	std::string sv( strval );
	long long val;

	if( from_string < long long >( val, sv, std::dec ) )
		return val;
	else
		return -1;
}

bool split_string_path( const std::string & original, std::string & left, std::string & right )
{
	bool bSuccess = false;
	std::string::size_type pos = original.find( '/' );
	if ( pos != std::string::npos )
	{
		left = original.substr(0, pos );
		right = original.substr( pos+1 );
		bSuccess = true;
	}
	return bSuccess;
}
