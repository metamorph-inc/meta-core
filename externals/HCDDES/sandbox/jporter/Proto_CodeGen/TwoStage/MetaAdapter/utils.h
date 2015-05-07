#ifndef UTILS_H
#define UTILS_H

#include <set>
#include <map>
#include <vector>
#include <stack>
#include <deque>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <sstream>
#include <fstream>

#include "ConfigKeeperS1.h"

class timeStamp
{
public:
	timeStamp(); // Set time to 'now'
	timeStamp( timeStamp & ts );
	virtual ~timeStamp() { delete _timeInfo; }  // Clean up

	std::string getDateTime(bool compact = false);
private:

	struct tm * _timeInfo;
};

//type-specific conversions
class conversionErr { 
public:
	conversionErr( std::string & input, std::string & convtype ) {
		_what = "Attempted to convert " + input + " using " + convtype + ".";
	}
	std::string _what;
};

/*** ContextTracker ******************************************************************************
 * Keeps track of the traversal path through the model.
 ************************************************************************************************/
class contextTracker
{
public:
	contextTracker() { }
	virtual ~contextTracker() { }

	void enter( std::string & ctxtname ) { _ctxtPath.push_back(ctxtname); std::cout << "In: " << getContext() << std::endl; }
	void leave() { std::cout << "Out: " << getContext() << std::endl; _ctxtPath.pop_back(); }
	std::string getContext() {
		std::ostringstream out;
		std::deque< std::string >::iterator pElts;
		for ( pElts = _ctxtPath.begin(); pElts != _ctxtPath.end(); pElts++ )
		{
			out << "_" << *pElts;
		}
		return out.str();
	}

private:
	std::string _ctxtName;
	std::deque< std::string > _ctxtPath;
};

class ctxt {
public:
	ctxt( contextTracker & ct, std::string cn ) : _ct( &ct ) {
		if ( ConfigKeeperS1::inst().getContextEnabled() ) _ct->enter( cn );
	}
	virtual ~ctxt() {
		if ( ConfigKeeperS1::inst().getContextEnabled() ) _ct->leave();
	}

private: 
	contextTracker * _ct;
};

double timeStr2secs( std::string & timeStr ) throw (conversionErr);
double freqStr2hz ( std::string & freqStr ) throw (conversionErr);
double rateStr2bps( std::string & rateStr ) throw (conversionErr);
unsigned long long sizeStr2bytes( std::string & sizeStr ) throw (conversionErr);

// other direction
std::string stringify( double val );
long rescaleval( double & val ); // clean up the value for output
std::string getUnits( long scale );
std::string secs2timeStr( double val );
std::string hz2freqStr( double val );
std::string bps2rateStr( double val );
std::string bytes2sizeStr( double val );

//general conversion functions
bool convertToDouble(const std::string& s, double& d, std::string& units);
bool convertToUnsignedLongLong(const std::string& s, unsigned long long & l, std::string& units);

//unit conversion functions
double FreqU2Val(std::string& frequnits );
double TimeU2Val(std::string& timeunits );
double RateU2Val(std::string& rateunits );
unsigned long long SizeU2Val(std::string& sizeunits );

//convert to lower-case
void toLower(std::string& str);


#endif//UTILS_H