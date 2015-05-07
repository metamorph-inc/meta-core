#ifndef __UTILS_H__
#define __UTILS_H__


/*** Included Header Files ***/
#include <set>
#include <map>
#include <list>
#include <vector>
#include <stack>
#include <deque>
#include <algorithm>
#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>


/***************************************************************************************/


double TimeStr2secs( std::string &timeStr );
double FreqStr2hz( const std::string &freqStr );
double RateStr2bps( const std::string &rateStr );
unsigned long long SizeStr2bytes( const std::string &sizeStr );
std::string Stringify( const double &value );
long RescaleValue( double & value );
std::string GetUnits( const long &scale );
std::string Secs2timeStr( const double &value );
std::string Hz2freqStr( const double &value );
std::string Bps2rateStr( const double &value );
std::string Bytes2sizeStr( const double &value );
void SplitNumUnits(const std::string &s, std::string &num, std::string &units);
void toLower( std::string &str);
bool ConvertToDouble( const std::string &s, double &d, std::string &units );
bool ConvertToUnsignedLongLong( const std::string &s, unsigned long long &l, std::string &units);
double FreqU2Val( const std::string &units );
double TimeU2Val( const std::string &units );
double RateU2Val(const std::string &units );
unsigned long long SizeU2Val(const std::string &units );
std::string ReplaceString( const std::string & source, const std::string & findstr, const std::string & replacestr );


/***************************************************************************************/


class ConversionErr { 
public:
	std::string				_what;
	ConversionErr( const std::string &input, const std::string &convType ) : _what() {
		this->_what = "Attempted to convert " + input + " using " + convType + ".";
	}
};


/***************************************************************************************/


class TimeStamp {
private:
	struct tm						*_timeInfo;
public:
	// Constructors and Destructors
	TimeStamp() : _timeInfo(NULL) {
		_timeInfo = new tm;
		time_t rawTime;
		time(&rawTime);
		this->_timeInfo = localtime(&rawTime);
	}
	TimeStamp( TimeStamp &timeStamp ) : _timeInfo(NULL) {
		// Allocate space for the new time struct
		this->_timeInfo = new tm;
		// Copy the memory
		memcpy(this->_timeInfo, timeStamp._timeInfo, sizeof( struct tm ) );
	}
	virtual ~TimeStamp()			{ /* delete _timeInfo; */ }

	// Accessor Methods
	std::string GetDateTime( const bool &compact=false ) {
		std::string format;
		if ( !compact ) {
			// Human-readable for printing (default)
			format = "%Y.%m.%d - %H:%M:%S";
		} else {
			// Compact form for use in a filename
			format = "%Y%m%d_%H%M%S";
		}
		char timeBuf[1024];
		strftime( timeBuf, 1024, format.c_str(), this->_timeInfo );
		return std::string( timeBuf );
	}
};


/***************************************************************************************/


// Keeps track of the traversal path through the model.
class ContextTracker {
private:
	std::string							_name;
	std::deque< std::string >			_pathQueue;
	bool                                _showCtxt;
public:
	ContextTracker() : _showCtxt(false) 	{ }
	virtual ~ContextTracker()			{ }

	void showContext( bool tf = true ) { _showCtxt = tf; }

	void Enter( const std::string &contextName )	{ 
		this->_pathQueue.push_back(contextName); 
		if( _showCtxt) 
			std::cout << "In: " << this->GetContext() << std::endl;
	}
	void Leave() {
		if( _showCtxt) 
			std::cout << "Out: " << this->GetContext() << std::endl;
		this->_pathQueue.pop_back();
	}
	std::string GetContext() {
		std::ostringstream out;
		std::deque< std::string >::iterator elemIter;
		for ( elemIter = _pathQueue.begin(); elemIter != _pathQueue.end(); elemIter++ ) {
			out << "_" << *elemIter;
		}
		return out.str();
	}
};


/***************************************************************************************/


inline double TimeStr2secs( std::string &timeStr ) {
	std::string units;
	double val;
	if (!ConvertToDouble( timeStr, val, units ))
		throw ConversionErr( timeStr, std::string("time") );
	return val * TimeU2Val( units );
}


inline double FreqStr2hz( const std::string &freqStr ) {
	std::string units;
	double val;
	if (!ConvertToDouble( freqStr, val, units ))
		throw ConversionErr( freqStr, std::string("frequency") );
	return val * FreqU2Val( units );
}


inline double RateStr2bps( const std::string &rateStr ) {
	std::string units;
	double val;
	if (!ConvertToDouble( rateStr, val, units ))
		throw ConversionErr( rateStr, std::string("rate") );
	return val * RateU2Val( units );
}


inline unsigned long long SizeStr2bytes( const std::string &sizeStr ) {
	std::string units;
	unsigned long long val;
	if (!ConvertToUnsignedLongLong( sizeStr, val, units ))
		throw ConversionErr( sizeStr, std::string("size") );
	return val * SizeU2Val( units );
}


inline std::string Stringify( const double &value ) {
	// Create an ostringstream
	std::ostringstream out;
	// Put value into the oss
	out << value;
	return out.str();
}


// Clean up the value for output
inline long RescaleValue( double & value ) {
	long scale = 0;
	double tmpVal = value;
	// Loop until tmpVal is bounded between 0 and 1
	while ( (tmpVal < 1.0) && (tmpVal > 0.0) ) {
		scale -= 3;
		tmpVal *= 1000.0;
		//std::cout << tmpVal << std::endl;
	}

	while ( tmpVal > 1000.0 ) {
		scale += 3;
		tmpVal /= 1000.0;
		//std::cout << tmpVal << std::endl;
	}
	value = tmpVal;
	return scale;
}


inline std::string GetUnits( const long &scale ) {
	std::string unitstr;
	if ( scale == 0 ) unitstr = "";
	else if ( scale == 3 ) unitstr = "k";
	else if ( scale == 6 ) unitstr = "M";
	else if ( scale == 9 ) unitstr = "G";
	else if ( scale == 12 ) unitstr = "P";
	else if ( scale == -3 ) unitstr = "m";
	else if ( scale == -6 ) unitstr = "u";
	else if ( scale == -9 ) unitstr = "n";
	else if ( scale == -12 ) unitstr = "p";
	return unitstr;
}


inline std::string Secs2timeStr( const double &value ) {
	double tmpVal = value;
	long scale = RescaleValue( tmpVal );
	return Stringify( tmpVal ) + GetUnits( scale ) + std::string("s");
}


inline std::string Hz2freqStr( const double &value ){
	double tmpVal = value;
	long scale = RescaleValue( tmpVal );
	return Stringify( tmpVal ) + GetUnits(scale) + std::string("Hz");	
}


inline std::string Bps2rateStr( const double &value ) {
	double tmpVal = value;
	long scale = RescaleValue( tmpVal );
	return Stringify( tmpVal ) + GetUnits(scale) + std::string("b");
}


inline std::string Bytes2sizeStr( const double &value ) {
	double tmpVal = value;
	long scale = RescaleValue( tmpVal );
	return Stringify( tmpVal ) + GetUnits(scale) + std::string("B");
}


inline void SplitNumUnits(const std::string &s, std::string &num, std::string &units) {
	int i = 0;
	// Find all number characters
	while ( ( (s[i]>='0') && (s[i]<='9') ) || s[i]=='.' ) i++;
	// Substring off just the number characters
	num = s.substr(0,i);
	// Now need to make sure there is no leading white space
	while ( s[i] == ' ' ) i++;
	// Now get just the non-whitespace porttion of the units
	units = s.substr(i);
	// Make sure units are always lower case
	toLower(units);
}


inline void toLower( std::string &str ) {
	for (int i =0; i!= str.length(); i++) {
		if (isalpha(str[i]))
			str[i] = tolower(str[i]);
	}
}


inline bool ConvertToDouble( const std::string &s, double &d, std::string &units ) {
	std::string num = "";
	units = "";
	SplitNumUnits( s, num, units );
	std::istringstream i( num );
	if (!(i >> d))
		return false;
	return true;
}


inline bool ConvertToUnsignedLongLong( const std::string &s, unsigned long long &l, std::string &units) {
	std::string num = "";
	units = "";
	SplitNumUnits( s, num, units );
	std::istringstream i( num );
	if (!(i >> l))
		return false;
	return true;
}


inline double FreqU2Val( const std::string &units ) {
	if (( units == "hz" ) || (units == ""))	return 1.0;
	else if ( units == "khz" )				return 1e3;
	else if ( units == "mhz" )				return 1e6;
	else if ( units == "ghz" )				return 1e9;
	return -1.0;
}


inline double TimeU2Val( const std::string &units ){
	if (( units == "s" ) || (units == ""))	return 1.0;
	else if ( units == "ms" )				return 1e-3;
	else if ( units == "us" )				return 1e-6;
	else if ( units == "ns" )				return 1e-9;
	else if ( units == "ps" )				return 1e-12;
	else if ( units == "fs" )				return 1e-15;
	return -1.0;
}


inline double RateU2Val(const std::string &units ) {
	if (( units == "b" ) || (units == ""))	return 1.0;
	else if ( units == "kb" )				return 1e3;
	else if ( units == "mb" )				return 1e6;
	else if ( units == "gb" )				return 1e9;
	return -1.0;
}


inline unsigned long long SizeU2Val(const std::string &units ) {
	if (( units == "b" ) || (units == ""))	return 1;
	else if ( units == "kb" )				return 1 * 1024;
	else if ( units == "mb" )				return 1 * 1024 * 1024;
	else if ( units == "gb" ) return (unsigned long long)1 * 
									 (unsigned long long)1024 * 
									 (unsigned long long)1024 * 
									 (unsigned long long)1024;
	return 0;
}

inline std::string ReplaceString( const std::string & source, 
								  const std::string & findstr, 
								  const std::string & replacestr ) {

	if ( findstr == replacestr )
		return source;

	std::string newstr( source );
	std::string::size_type pos = 0;
	while ( (pos = newstr.find(findstr, pos)) != std::string::npos ) {
        newstr.replace( pos, findstr.size(), replacestr );
        pos++;
    }

	return newstr;
}

/***************************************************************************************/


#endif //__UTILS_H__

