#ifndef ParseUtils_h__
#define ParseUtils_h__

#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>

// Utility Functions

// Split a string at the '/'
bool split_string_path( const std::string & original, 
						std::string & left, std::string & right );

// Get something of type T from a string
// we assume that >> already knows what to do for type T
template < class T >
bool from_string( T & t,
				 const std::string & s,
				 std::ios_base & ( *f ) ( std::ios_base & ) ) {
					 std::istringstream iss( s );
					 return !( iss >> f >> t ).fail(  );
}

double to_double( char * strval );
long long to_integer( char * strval );

inline double FreqU2Val( const std::string & units ) {
	std::string convunits( units );
	std::transform( units.begin(), units.end(), convunits.begin(), std::tolower );
	if (( convunits == "hz" ) || (convunits == ""))	return 1.0;
	else if ( convunits == "khz" )				return 1e3;
	else if ( convunits == "mhz" )				return 1e6;
	else if ( convunits == "ghz" )				return 1e9;
	return -1.0;
}

inline double FreqU2Val( const char * units ) {
	return FreqU2Val( std::string( units ) );
}

inline double TimeU2Val( const std::string & units ){
	std::string convunits( units );
	std::transform( units.begin(), units.end(), convunits.begin(), std::tolower );
	if (( convunits == "s" ) || (convunits == ""))	return 1.0;
	else if ( convunits == "ms" )				return 1e-3;
	else if ( convunits == "us" )				return 1e-6;
	else if ( convunits == "ns" )				return 1e-9;
	else if ( convunits == "ps" )				return 1e-12;
	else if ( convunits == "fs" )				return 1e-15;
	return -1.0;
}

inline double TimeU2Val( const char * units ) {
	return TimeU2Val( std::string( units ) );
}

inline double RateU2Val(const std::string & units ) {
	std::string convunits( units );
	std::transform( units.begin(), units.end(), convunits.begin(), std::tolower );
	if (( convunits == "b" ) || (convunits == ""))	return 1.0;
	else if ( convunits == "kb" )				return 1e3;
	else if ( convunits == "mb" )				return 1e6;
	else if ( convunits == "gb" )				return 1e9;
	return -1.0;
}

inline double RateU2Val( const char * units ) {
	return RateU2Val( std::string( units ) );
}

inline unsigned long long SizeU2Val(const std::string & units ) {
	std::string convunits( units );
	std::transform( units.begin(), units.end(), convunits.begin(), std::tolower );
	if (( convunits == "b" ) || (convunits == ""))	return 1;
	else if ( convunits == "kb" )				return 1 * 1024;
	else if ( convunits == "mb" )				return 1 * 1024 * 1024;
	else if ( convunits == "gb" )				return (unsigned long long) 1 * 
													(unsigned long long) 1024 * 
													(unsigned long long) 1024 * 
													(unsigned long long) 1024;
	return 0;
}

inline unsigned long long SizeU2Val( const char * units ) {
	return SizeU2Val( std::string( units ) );
}

#endif // ParseUtils_h__