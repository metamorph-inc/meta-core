#ifndef _LOCAL_LOG_H_
#define _LOCAL_LOG_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <map>

#include "SchedTemplates/schedGen.h"

// It would have been nice to use a separate file output stream
// for each DesignFolder, but VS 2005 is broken with respect to
// having the ofstream objects in another structure.  Therefore
// we have to use the ForBlock in the transformation and make
// sure this is only accessed sequentially
extern std::ofstream out;
extern std::map<std::string, SchedGen> sgs;

double unscaleval( char scale, double val );
double rescaleval( char * scale, double val );
std::string AddTimeAsString( std::string & a1, std::string & a2 );
std::string InvertTimeAsString( std::string & val );
std::string UniqueId2Str( long uid );

#endif // _LOCAL_LOG_H_
