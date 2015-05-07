#include "utils.h"

#include <cstdlib>

#pragma warning(disable:4996)
#pragma warning(disable:4267)

using namespace std;

timeStamp::timeStamp()
{
	_timeInfo = 0;
	time_t rawTime;
	time(&rawTime);
	_timeInfo = localtime(&rawTime);
}

timeStamp::timeStamp( timeStamp & ts)
{
	_timeInfo = new tm; // make sure we copy the memory
	memcpy(_timeInfo, ts._timeInfo, sizeof( struct tm));
}

std::string timeStamp::getDateTime(bool compact)
{
	string fmt;
	if ( !compact )
	{
		fmt = "%Y.%m.%d - %H:%M:%S"; // Human-readable for printing (default)
	}
	else
	{
		fmt = "%Y%m%d_%H%M%S"; // Compact form for use in a filename
	}

	char timeBuf[1024];
	strftime(timeBuf, 1024, fmt.c_str(), _timeInfo);
	return std::string( timeBuf );
}

double timeStr2secs( std::string & timeStr ) throw (conversionErr)
{
	std::string units;
	double val;
	if (!convertToDouble( timeStr, val, units ))
		throw conversionErr( timeStr, std::string("time") );

	return val * TimeU2Val( units );
}

double freqStr2hz ( std::string & freqStr ) throw (conversionErr)
{
	std::string units;
	double val;
	if (!convertToDouble( freqStr, val, units ))
		throw conversionErr( freqStr, std::string("frequency") );

	return val * FreqU2Val( units );
}

double rateStr2bps( std::string & rateStr ) throw (conversionErr)
{
	std::string units;
	double val;
	if (!convertToDouble( rateStr, val, units ))
		throw conversionErr( rateStr, std::string("rate") );

	return val * RateU2Val( units );
}

unsigned long long sizeStr2bytes( std::string & sizeStr ) throw (conversionErr)
{
	std::string units;
	unsigned long long val;
	if (!convertToUnsignedLongLong( sizeStr, val, units ))
		throw conversionErr( sizeStr, std::string("size") );

	return val * SizeU2Val( units );
}

std::string stringify( double val )
{
	std::ostringstream out;
	out << val;
	return out.str();
}

long rescaleval( double & val ) // clean up the value for output
{
	long scale = 0;
	double tmpval = val;

	while ( (tmpval < 1.0) && (tmpval > 0.0) )
	{
		scale -= 3;
		tmpval *= 1000.0;
		cout << tmpval << endl;
	}

	while ( tmpval > 1000.0 )
	{
		scale += 3;
		tmpval /= 1000.0;
		cout << tmpval << endl;
	}

	val = tmpval;
	return scale;
}

std::string getUnits( long scale )
{
	string unitstr;
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

std::string secs2timeStr( double val )
{
	double tmpval = val;
	long scale = rescaleval( tmpval );
	return stringify( tmpval ) + getUnits(scale) + string("s");
}

std::string hz2freqStr( double val )
{
	double tmpval = val;
	long scale = rescaleval( tmpval );
	return stringify( tmpval ) + getUnits(scale) + string("Hz");	
}

std::string bps2rateStr( double val )
{
	double tmpval = val;
	long scale = rescaleval( tmpval );
	return stringify( tmpval ) + getUnits(scale) + string("b");
}

std::string bytes2sizeStr( double val )
{
	double tmpval = val;
	long scale = rescaleval( tmpval );
	return stringify( tmpval ) + getUnits(scale) + string("B");
}

void splitNumUnits(const std::string& s, std::string& num, std::string& units)
{
	int i = 0;
	for(i=0; i!=s.length(); i++)
	{
		if (((s[i]>='0')&&(s[i]<='9'))||s[i]=='.')
			continue;
		break;
	}
	num = s.substr(0,i);
	units = s.substr(i);
	toLower(units);
}
void toLower(std::string& str)
{
	for (int i =0; i!= str.length(); i++)
	{
		if (isalpha(str[i]))
			str[i] = tolower(str[i]);
	}
}

bool convertToDouble(const std::string& s, double& d, std::string& units)
{
	std::string num="";
	units ="";
	splitNumUnits(s,num,units);
	std::istringstream i(num);
	if (!(i >> d))
		return false;
	return true;
}

bool convertToUnsignedLongLong(const std::string& s, unsigned long long& l, std::string& units)
{
	std::string num="";
	units ="";
	splitNumUnits(s,num,units);
	std::istringstream i(num);
	if (!(i >> l))
		return false;
	return true;
}



double FreqU2Val(std::string& units)
{
	
	if (( units == "hz" ) || (units == ""))
		return 1.0;
	else if ( units == "khz" )
		return 1e3;
	else if ( units == "mhz" )
		return 1e6;
	else if ( units == "ghz" )
		return 1e9;
	return -1.0;
}

double TimeU2Val(std::string& units)
{

	if (( units == "s" ) || (units == ""))
		return 1.0;
	else if ( units == "ms" )
		return 1e-3;
	else if ( units == "us" )
		return 1e-6;
	else if ( units == "ns" )
		return 1e-9;
	else if ( units == "ps" )
		return 1e-12;
	else if ( units == "fs" )
		return 1e-15;
	return -1.0;
}



double RateU2Val(std::string& units)
{

	if (( units == "b" ) || (units == ""))
		return 1.0;
	else if ( units == "kb" )
		return 1e3;
	else if ( units == "mb" )
		return 1e6;
	else if ( units == "gb" )
		return 1e9;
	return -1.0;
}

unsigned long long SizeU2Val(std::string& units)
{
	
	if (( units == "b" ) || (units == ""))
		return 8;
	else if ( units == "kb" )
		return 8 * 1024;
	else if ( units == "mb" )
		return 8 * 1024 * 1024;
	else if ( units == "gb" )
		return (unsigned long long) 8 
			* (unsigned long long) 1024 
			* (unsigned long long) 1024 
			* (unsigned long long) 1024;
	return 0;
}

