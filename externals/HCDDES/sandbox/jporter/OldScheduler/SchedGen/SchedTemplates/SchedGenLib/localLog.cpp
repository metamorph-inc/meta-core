#include <cstdio>
#include <cmath>

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
// sure the out field is only accessed sequentially


std::ofstream out;
std::map<std::string, SchedGen> sgs;


double unscaleval( char scale, double val )
{
	switch (scale)
	{
		case 'f' : val = val / 1000.0;
		case 'p' : val = val / 1000.0;
		case 'n' : val = val / 1000.0;
		case 'u' : val = val / 1000.0;
		case 'm' : val = val / 1000.0;
			break;
		case 'T' : val *= 1000.0;
		case 'G' : val *= 1000.0;
		case 'M' : val *= 1000.0;
		case 'k' : val *= 1000.0;
			break;
		default:
			;
	}

	return val;
}

double rescaleval( char * scale, double val )
{
	int count = 0;
	while ((fabs(val) > 0.0) && (fabs(val) < 1000.0) && (count < 5))
	{
		val *= 1000.0;
		count++;
	}

	*scale = '0';
	if ( count == 1 )
		*scale='m';
	else if ( count == 2 )
		*scale='u';
	else if ( count == 3 )
		*scale='n';
	else if ( count == 4 )
		*scale='p';
	else if ( count == 5 )
		*scale='f';

	count = 0;
	while ( fabs(val) > 1000.0 && count < 4 )
	{
		val = val / 1000.0;
		count++;
	}

	if ( count == 1 )
		*scale = 'k';
	else if ( count == 2 )
		*scale='M';
	else if ( count == 3 )
		*scale='G';
	else if ( count == 4 )
		*scale='T';

	return val;
}

std::string AddTimeAsString( std::string & a1, std::string & a2 )
{
	// Convert both strings to double (in seconds), add, then convert back
	
	double v1 = 0.0, v2 = 0.0;
	char scale1[2], scale2[2];

	if ( a1.size() != 0 )
	{
		std::sscanf( a1.c_str(), " %Lf %1s%*s", &v1, scale1 );
		v1 = unscaleval( scale1[0], v1 );	
	}

	if ( a2.size() != 0 )
	{
		std::sscanf( a2.c_str(), " %Lf %1s%*s", &v2, scale2 );
		v2 = unscaleval( scale2[0], v2 );
	}

	char rescale;
	double result = rescaleval( &rescale, v1+v2 );

	std::ostringstream out;
	out << result;
	if ( rescale != '0' )
		out << rescale;
	out << "s";

	return out.str(); // return by value -- should be OK
}

std::string InvertTimeAsString( std::string & val )
{
	std::ostringstream out;

	if ( val.size() != 0 )
	{
		double v;
		char scale[2];

		std::sscanf( val.c_str(), " %Lf %1s%*s", &v, scale );
		v = unscaleval( scale[0], v );

		char rescale;
		double result;
		
		if ( fabs(v) > 0.0 )
		{
			result = rescaleval( &rescale, 1.0/v );
		}
		else
		{
			result = 0.0;
			rescale = '0';
		}
		
		out << result;
		if ( rescale != '0' )
			out << rescale;
	}
	else
	{
		out << "0";
	}
	out << "Hz";

	return out.str(); // return by value -- should be OK
}

std::string UniqueId2Str( long uid )
{
	std::ostringstream out;
	out << uid;
	return std::string( out.str() );
}